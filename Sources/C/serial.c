/*
 * serial.c
 *
 *  Created on: 2 nov. 2017
 *      Author: dene2303
 *
 *  Serial  communication functions
 */

#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_edma.h>
#include <csl_dma.h>
#include <csl_irq.h>
#include <stdio.h>

#include "serial.h"
#include "addresses.h"
#include "constants.h"
#include "math.h"

#define X_IntCode 10
#define R_IntCode 11

void ConfigEDMA();
void ConfigMcBSP(BaudRate baud);
short VoteLogic(unsigned short value);
char ProcessReceiveData(void);

/* Create buffers and aligning them on an L2 cache line boundary. */
unsigned short xmitbuf[0x0400];
unsigned short recvbuf[0x0400];

/* Definitions  */
#define BUFFER_SIZE 16              /* total number of UART data words  */

/* Declare CSL objects */
MCBSP_Handle hMcbsp;

EDMA_Handle hEdmaX;
EDMA_Handle hEdmaR;


// Receive callback function
void (*sendCharBackFunction)(char);


int setupSerial (McBSP_Port port, BaudRate baud) {

    printf("Starting serial setup\n");

    /* initialize the CSL library */
    CSL_init();

    /* enable NMI and GI */
    IRQ_nmiEnable();
    IRQ_globalEnable();

    /* Open the EDMA channels - EDMA  for transmit, EDMA  for receive   */
    hEdmaR = EDMA_open(port == McBSP0 ? EDMA_CHA_REVT0 : EDMA_CHA_REVT1, EDMA_OPEN_RESET);


    /* Open the McBSP channel 1 */
    hMcbsp = MCBSP_open(port == McBSP0 ? MCBSP_DEV0 : MCBSP_DEV1, MCBSP_OPEN_RESET);

    /* Configure the EDMA channels */
    ConfigEDMA();

    /* enable EDMA-CPU interrupt tied to McBSP */
    IRQ_enable(IRQ_EVT_EDMAINT);
    /* enable EDMA channel interrupt to CPU */

    EDMA_intEnable(port == McBSP0 ? 13 : 15);  // Receive

    EDMA_enableChannel(hEdmaR);

    /* Setup for McBSP */
    ConfigMcBSP(baud);

    /* Start Sample Rate Generator: set /GRST = 1 */
    MCBSP_enableSrgr(hMcbsp);

    int i;
    /* inserted wait time for McBSP to get ready */
    for (i=0; i<0xF; ++i);

    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(hMcbsp);

    printf("Finished serial setup\n");

    return 0;
}

void ConfigMcBSP(BaudRate baud)
{

    MCBSP_Config mcbspCfg = {

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


        MCBSP_RCR_RMK(
            MCBSP_RCR_RPHASE_DUAL,          /* 1     */
            MCBSP_RCR_RFRLEN2_OF(1),        /* 00010 */
            MCBSP_RCR_RWDLEN2_8BIT,         /* 000   */
            MCBSP_RCR_RCOMPAND_MSB,         /* 00    */
            MCBSP_RCR_RFIG_YES,             /* 1     */
            MCBSP_RCR_RDATDLY_1BIT,         /* 01    */
            MCBSP_RCR_RFRLEN1_OF(8),        /* 01000 */
            MCBSP_RCR_RWDLEN1_16BIT,        /* 010   */
            MCBSP_RCR_RWDREVRS_DISABLE      /* 0     */
        ),


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
            MCBSP_SRGR_GSYNC_FREE,          /* 0      */
            MCBSP_SRGR_CLKSP_RISING,        /* 0      */
            MCBSP_SRGR_CLKSM_INTERNAL,      /* 1      */
            MCBSP_SRGR_FSGM_DXR2XSR,        /* 0      */
            MCBSP_SRGR_FPER_DEFAULT,        /* 0      */
            MCBSP_SRGR_FWID_DEFAULT,        /* 0      */
            // clockDividerValue = roundf(CLK_SRC / ((float)baud * 16.0f)) - 1;
            MCBSP_SRGR_CLKGDV_OF(243)       // 57600 baud at 225 MHz

        ),


        MCBSP_MCR_DEFAULT,                  /* default values */
        MCBSP_RCER_DEFAULT,                 /* default values */
        MCBSP_XCER_DEFAULT,                 /* default values */


        /* PCR Setup */
        MCBSP_PCR_RMK(
            MCBSP_PCR_XIOEN_SP,             /* 0 */
            MCBSP_PCR_RIOEN_SP,             /* 0 */
            MCBSP_PCR_FSXM_INTERNAL,        /* 1 */
            MCBSP_PCR_FSRM_EXTERNAL,        /* 0 */
            MCBSP_PCR_CLKXM_OUTPUT,         /* 1 */
            MCBSP_PCR_CLKRM_OUTPUT,         /* 1 */
            MCBSP_PCR_CLKSSTAT_0,           /* 0 */
            MCBSP_PCR_DXSTAT_0,             /* 0 */
            MCBSP_PCR_FSXP_ACTIVELOW,       /* 1 */
            MCBSP_PCR_FSRP_ACTIVELOW,       /* 1 */
            MCBSP_PCR_CLKXP_RISING,         /* 0 */
            MCBSP_PCR_CLKRP_FALLING         /* 0 */
        )
    };

    MCBSP_config(hMcbsp, &mcbspCfg);

}   /* end of Config_McBSP(void) */

void ConfigEDMA()
{

    EDMA_configArgs(hEdmaR,
        EDMA_OPT_RMK(
            EDMA_OPT_PRI_HIGH,      /* 1  */
            EDMA_OPT_ESIZE_16BIT,   /* 01 */
            EDMA_OPT_2DS_NO,        /* 0  */
            EDMA_OPT_SUM_NONE,      /* 00 */
            EDMA_OPT_2DD_NO,        /* 0  */
            EDMA_OPT_DUM_INC,       /* 01 */
            EDMA_OPT_TCINT_YES,     /* 1  */
            EDMA_OPT_TCC_OF(R_IntCode),
            EDMA_OPT_LINK_NO,       /* 0  */
            EDMA_OPT_FS_NO          /* 0  */
        ),

        /* SRC Setup */
        EDMA_SRC_RMK(MCBSP_getRcvAddr(hMcbsp)),

        /* CNT Setup */
        EDMA_CNT_RMK(0, (BUFFER_SIZE * 11)),

        /* DST Setup */
        EDMA_DST_RMK((Uint32) recvbuf),        /*recvbuf address*/

        /* IDX Setup */
        EDMA_IDX_RMK(0,0),

        /* RLD Setup */
        EDMA_RLD_RMK(0,0)
    );

} /* End of ConfigEDMA() */


char ProcessReceiveData(void)
{
    unsigned char recv_char = 0;
    short cnt = -1;
    short recv_val;
    unsigned short  raw_data;
    unsigned short  *recvbufptr;    /*receive buffer pointer*/

    /* Point to the receive buffer  */
    recvbufptr  = (unsigned short *)recvbuf;

    /* Process all data in the Receive buffer   */

    recv_char = 0;

    /* Process each UART frame  */
    for (cnt = -1; cnt < 10; cnt++)
    {
        if(cnt == -1 || cnt == 8 || cnt == 9)
        {
            /* Ignore Start and Stop bits   */
            recvbufptr++;
        }
        else
        {
            /* Get 16-bit data from receive buffer  */
            raw_data    =   *recvbufptr;
            recvbufptr++;

            /* get the value of the majority of the bits    */
            recv_val    =   VoteLogic(raw_data);

            /* put received bit into proper place   */
            recv_char   += recv_val << cnt;
        }
    }   /* end for cnt  */

    /* A full BYTE is decoded. Put in result: recv_msg[i]   */
    return recv_char;
}   /* end ProcessReceiveData() function    */

/*******************************************************************/
/* short VoteLogic(unsigned short)                                 */
/*                                                                 */
/* This function decoded the received character by testing the     */
/* center 4 bits of the baud.  A majority rule is used for the     */
/* decoding.                                                       */
/*******************************************************************/
short VoteLogic(unsigned short value)
{
    short returnvalue;

    switch  ((value >>  6)  & 0x0F)
    {
        case    0:
        case    1:
        case    2:
        case    3:
        case    4:
        case    5:
        case    6:
        case    8:
        case    9:
        case    10:
                        returnvalue = 0;
                        break;
        case    7:
        case    11:
        case    12:
        case    13:
        case    14:
        case    15:
                        returnvalue = 1 ;
                        break;
    }   /* end switch   */

    return (returnvalue);

}   /* end VoteLogic() funciton */

int setReceiveFunction(void (*callback)(char)){
    sendCharBackFunction = callback;
    return 0;
}

interrupt void c_int08(void)
{
    if (EDMA_intTest(R_IntCode))
    {

        sendCharBackFunction(ProcessReceiveData());


        EDMA_intClear(R_IntCode);
        printf("Byte received\n");
    }
}
