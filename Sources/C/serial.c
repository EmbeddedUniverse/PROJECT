#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_edma.h>
#include <csl_dma.h>
#include <csl_irq.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dsk6713.h>
#include "serial.h"
#include "addresses.h"
#include "constants.h"
#include "math.h"


void ConfigEDMA();
void ConfigMcBSP();
short VoteLogic(unsigned short value);
void ProcessReceiveData(void);


extern far void vectors();

const unsigned short FRAME_LENGTH = 9;
const int RECV_INT_CODE_0 = EDMA_CHA_REVT0;
const int RECV_INT_CODE_1 = EDMA_CHA_REVT1;

char *recvBuff;
unsigned short *recvBitsBuff;
unsigned int recvBuffLength;
unsigned int recvBitsLength;


MCBSP_Handle hMcbsp;
EDMA_Handle hEdmaR;
McBSP_Port selectedPort;
BaudRate selectedBaud;

void (*sendCharBackFunction)(const char*, size_t);



int setupSerial (
        McBSP_Port port,
        BaudRate baud,
        unsigned int expectedStringLength,
        void (*callBack)(const char*, size_t))
{
    selectedBaud = baud;
    selectedPort = port;

    recvBuffLength = expectedStringLength;
    recvBitsLength = expectedStringLength * FRAME_LENGTH;
    recvBitsBuff = (unsigned short*) malloc(recvBitsLength * sizeof(unsigned short));
    recvBuff = (char*) malloc((recvBuffLength + 1) * sizeof(char));
    memset(recvBuff, '\0', (recvBuffLength + 1) * sizeof(char));

    // Rerouting McBSP to daughter board
    DSK6713_rset(DSK6713_MISC, port == McBSP0 ? 1 : 2);

    /* enable NMI and GI */
    IRQ_nmiEnable();
    IRQ_globalEnable();

    /* point to the IRQ vector table */
    IRQ_setVecs(vectors);   // CCS Error does not result in compilation fault

    /* disable and clear the event interrupt */
    IRQ_reset(IRQ_EVT_EDMAINT);

    /* clear Parameter RAM of EDMA */
    EDMA_clearPram(0x00000000);

    /* Open the EDMA channels - EDMA  for transmit, EDMA  for receive   */
    hEdmaR = EDMA_open(port == McBSP0 ? EDMA_CHA_REVT0 : EDMA_CHA_REVT1, EDMA_OPEN_RESET);

    /* Open the McBSP channel 1 */
    hMcbsp = MCBSP_open(port == McBSP0 ? MCBSP_DEV0 : MCBSP_DEV1, MCBSP_OPEN_RESET);

    /* Configure the EDMA channels */
    ConfigEDMA();

    /* enable EDMA-CPU interrupt tied to McBSP */
    IRQ_enable(IRQ_EVT_EDMAINT);
    EDMA_intEnable(port == McBSP0 ? RECV_INT_CODE_0 : RECV_INT_CODE_1);  // Receive

    EDMA_enableChannel(hEdmaR);

    /* Setup for McBSP */
    ConfigMcBSP();

    /* Start Sample Rate Generator: set /GRST = 1 */
    MCBSP_enableSrgr(hMcbsp);

    int i;
    /* inserted wait time for McBSP to get ready */
    for (i=0; i<0xF; ++i);

    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(hMcbsp);

    setReceiveFunction(callBack);

    return 0;
}

void ConfigMcBSP()
{
    int clockDividerValue = roundf(CLK_SRC / ((float)selectedBaud * 32.0f)) - 1;

    MCBSP_configArgs(hMcbsp,
        // SPCR
        MCBSP_SPCR_RMK(
            MCBSP_SPCR_FREE_YES,            /* 1  */
            MCBSP_SPCR_SOFT_DEFAULT,        /* 0  */
            MCBSP_SPCR_FRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_GRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_XINTM_XRDY,          /* 00 */
            MCBSP_SPCR_XSYNCERR_DEFAULT,    /* 0  */
            MCBSP_SPCR_XRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_DLB_OFF,             /* 0  */
            MCBSP_SPCR_RJUST_RZF,           /* 00 */
            MCBSP_SPCR_CLKSTP_DISABLE,      /* 0  */
            MCBSP_SPCR_DXENA_OFF,           /* 0  */
            MCBSP_SPCR_RINTM_RRDY,          /* 00 */
            MCBSP_SPCR_RSYNCERR_DEFAULT,    /* 0  */
            MCBSP_SPCR_RRST_DEFAULT         /* 0  */
        ),

        /* RCR Setup */
        MCBSP_RCR_RMK(
            MCBSP_RCR_RPHASE_SINGLE,
            MCBSP_RCR_RFRLEN2_OF(1),
            MCBSP_RCR_RWDLEN2_DEFAULT,
            MCBSP_RCR_RCOMPAND_MSB,
            MCBSP_RCR_RFIG_YES,
            MCBSP_RCR_RDATDLY_1BIT,
            MCBSP_RCR_RFRLEN1_OF(FRAME_LENGTH - 1),
            MCBSP_RCR_RWDLEN1_16BIT,
            MCBSP_RCR_RWDREVRS_DISABLE
        ),

        /* XCR Setup */
        MCBSP_XCR_RMK(
            MCBSP_XCR_XPHASE_DUAL,          /* 1     */
            MCBSP_XCR_XFRLEN2_OF(1),        /* 00010 */
            MCBSP_XCR_XWDLEN2_8BIT,         /* 000   */
            MCBSP_XCR_XCOMPAND_MSB,         /* 00    */
            MCBSP_XCR_XFIG_YES,             /* 1     */
            MCBSP_XCR_XDATDLY_0BIT,         /* 00    */
            MCBSP_XCR_XFRLEN1_OF(8),        /* 01000 */
            MCBSP_XCR_XWDLEN1_16BIT,        /* 010   */
            MCBSP_XCR_XWDREVRS_DISABLE      /* 0     */
        ),

        /* SRGR Setup */
        MCBSP_SRGR_RMK(
            MCBSP_SRGR_GSYNC_FREE,              /* 0      */
            MCBSP_SRGR_CLKSP_RISING,            /* 0      */
            MCBSP_SRGR_CLKSM_INTERNAL,          /* 1      */
            MCBSP_SRGR_FSGM_DXR2XSR,            /* 0      */
            MCBSP_SRGR_FPER_DEFAULT,            /* 0      */
            MCBSP_SRGR_FWID_DEFAULT,            /* 0      */
            MCBSP_SRGR_CLKGDV_OF(121)           /* CLKGDV */
        ),

        /* MCR Setup */
        MCBSP_MCR_DEFAULT,                      /* default values */

        /* RCER Setup */
        MCBSP_RCER_DEFAULT,                     /* default values */


        /* XCER Setup */
        MCBSP_XCER_DEFAULT,                     /* default values */

        /* PCR Setup */
        MCBSP_PCR_RMK(
        MCBSP_PCR_XIOEN_SP,             /* 0 */
        MCBSP_PCR_RIOEN_SP,             /* 0 */
        MCBSP_PCR_FSXM_INTERNAL,            /* 1 */
        MCBSP_PCR_FSRM_EXTERNAL,            /* 0 */
        MCBSP_PCR_CLKXM_OUTPUT,             /* 1 */
        MCBSP_PCR_CLKRM_OUTPUT,             /* 1 */
        MCBSP_PCR_CLKSSTAT_0,               /* 0 */
        MCBSP_PCR_DXSTAT_0,             /* 0 */
        MCBSP_PCR_FSXP_ACTIVELOW,           /* 1 */
        MCBSP_PCR_FSRP_ACTIVELOW,           /* 1 */
        MCBSP_PCR_CLKXP_RISING,             /* 0 */
        MCBSP_PCR_CLKRP_FALLING             /* 0 */
        )
    );

}   /* end of Config_McBSP(void) */

void ConfigEDMA()
{
    // A table where to store the EDMA paramaters
    EDMA_Handle recvEdmaTable = EDMA_allocTable(EDMA_ALLOC_ANY);

    // The EDMA config
    EDMA_Config recvEdmaConfig;
    recvEdmaConfig.opt = EDMA_OPT_RMK(
        EDMA_OPT_PRI_HIGH,
        EDMA_OPT_ESIZE_16BIT,
        EDMA_OPT_2DS_NO,
        EDMA_OPT_SUM_NONE,
        EDMA_OPT_2DD_NO,
        EDMA_OPT_DUM_INC,
        EDMA_OPT_TCINT_YES,
        EDMA_OPT_TCC_OF(
            selectedPort == McBSP0 ? RECV_INT_CODE_0 : RECV_INT_CODE_1),
        EDMA_OPT_LINK_YES,
        EDMA_OPT_FS_NO);
    recvEdmaConfig.src = MCBSP_getRcvAddr(hMcbsp);
    recvEdmaConfig.cnt = EDMA_CNT_RMK(0, recvBitsLength);
    recvEdmaConfig.dst = (Uint32) recvBitsBuff;
    recvEdmaConfig.idx = EDMA_IDX_RMK(0,0);
    recvEdmaConfig.rld = EDMA_RLD_RMK(recvBitsLength, recvEdmaTable);

    EDMA_config(recvEdmaTable, &recvEdmaConfig);
    EDMA_config(hEdmaR, &recvEdmaConfig);
}


void ProcessReceiveData(void)
{
    int i, j;
    unsigned char recv_char;
    unsigned short  *recvbufptr;    /*receive buffer pointer*/

    /* Point to the receive buffer  */
    recvbufptr  = (unsigned short *)recvBitsBuff;

    /* Process all data in the Receive buffer   */
    for (i = 0; i < recvBuffLength; ++i)
    {
        recv_char = 0;
        ++recvbufptr;

        /* Process each UART frame  */
        for (j = 0; j < 8; ++j)
        {
            /* put received bit into proper place   */
            recv_char |= VoteLogic(*recvbufptr++) << j;
        }

        /* A full BYTE is decoded. Put in result: recv_msg[i]   */
        recvBuff[i] = recv_char;
    }
}

/*******************************************************************/
/* short VoteLogic(unsigned short)                                 */
/*                                                                 */
/* This function decoded the received character by testing the     */
/* center 4 bits of the baud.  A majority rule is used for the     */
/* decoding.                                                       */
/*******************************************************************/
short VoteLogic(unsigned short value)
{
    switch  ((value >>  6)  & 0x0F)
    {
        case 15: // 0b1111
        case 7:  // 0b0111
        case 14: // 0b1110
        case 11: // 0b1011
        case 13: // 0b1101
            return 1;
    }

    return 0;
}

int setReceiveFunction(void (*callback)(const char *, size_t)){
    sendCharBackFunction = callback;
    return 0;
}

interrupt void c_int08(void)
{
    int expectedInterruptCode;
    if (selectedPort == McBSP0)
        expectedInterruptCode = RECV_INT_CODE_0;
    else if (selectedPort == McBSP1)
        expectedInterruptCode = RECV_INT_CODE_1;
    else
        return;

    if (EDMA_intTest(expectedInterruptCode))
    {
        // Decode bits
        ProcessReceiveData();

        // Call Callback Function
        if (sendCharBackFunction)
            sendCharBackFunction(recvBuff, recvBuffLength);

        // Clear Interupt
        EDMA_intClear(expectedInterruptCode);
    }
}
