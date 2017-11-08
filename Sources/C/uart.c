/*******************************************************************/
/* THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR     */
/* REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,          */
/* INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS    */
/* FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR          */
/* COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.      */
/* TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET      */
/* POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY             */
/* INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR      */
/* YOUR USE OF THE PROGRAM.                                        */
/*                                                                 */
/* IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,     */
/* CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY       */
/* THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED      */
/* OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT      */
/* OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.     */
/* EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF       */
/* REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS     */
/* OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF       */
/* USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S          */
/* AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF      */
/* YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS             */
/* (U.S.$500).                                                     */
/*                                                                 */
/* Unless otherwise stated, the Program written and copyrighted    */
/* by Texas Instruments is distributed as "freeware".  You may,    */
/* only under TI's copyright in the Program, use and modify the    */
/* Program without any charge or restriction.  You may             */
/* distribute to third parties, provided that you transfer a       */
/* copy of this license to the third party and the third party     */
/* agrees to these terms by its first use of the Program. You      */
/* must reproduce the copyright notice and any other legend of     */
/* ownership on each copy or partial copy, of the Program.         */
/*                                                                 */
/* You acknowledge and agree that the Program contains             */
/* copyrighted material, trade secrets and other TI proprietary    */
/* information and is protected by copyright laws,                 */
/* international copyright treaties, and trade secret laws, as     */
/* well as other intellectual property laws.  To protect TI's      */
/* rights in the Program, you agree not to decompile, reverse      */
/* engineer, disassemble or otherwise translate any object code    */
/* versions of the Program to a human-readable form.  You agree    */
/* that in no event will you alter, remove or destroy any          */
/* copyright notice included in the Program.  TI reserves all      */
/* rights not specifically granted under this license. Except      */
/* as specifically provided herein, nothing in this agreement      */
/* shall be construed as conferring by implication, estoppel,      */
/* or otherwise, upon you, any license or other right under any    */
/* TI patents, copyrights or trade secrets.                        */
/*                                                                 */
/* You may not use the Program in non-TI devices.                  */
/*******************************************************************/


/*******************************************************************/
/* TEXAS INSTRUMENTS, INC.                                         */
/* Date Created: 06/22/2001                                        */
/* Date Last Modified: 07/9/2001                                   */
/* Source File: uart.c                                             */
/* Original Author: Todd Hiers                                     */
/* Author: Scott Chen                                              */
/*                                                                 */
/* This code describes how to initialize the C6000 McBSP to        */
/* communicate with an UART.  By modifying the CHIP definition,    */
/* this code can be used to run on 6x1x/6x0x/64x.  #if statements  */
/* are included in this code which allow flexibility in different  */
/* devices.                                                        */
/*                                                                 */
/* On 6x0x devices, DMA channels 1 and 2 are used to service       */
/* McBSP 1 transmit and receive operations, respectively.  On      */
/* 6x1x/64x devices, EDMA channels 14 and 15 are used to service   */
/* McBSP 1 transmit and receive operations, respectively.          */
/*                                                                 */
/* For this example, a data string is being transmitted from McBSP */
/* transmit (DX) to McBSP receive (DR).  Each bit of the 8-bit     */
/* ASCII character is expanded into 16-bit UART transmission word. */
/* Once being received, the 16-bit UART transmission words are     */
/* compressed back to binary bits and ASCII form.                  */
/*                                                                 */
/* For the code to work, DX, DR, and FSR of McBSP1 are shorted     */
/* together.                                                       */
/*                                                                 */
/* This code has been verified for functionality on 6711, 6202,    */
/* and 6203 devices.                                               */
/*                                                                 */
/* This program is based on CSL 2.0.  Please refer to the          */
/* TMS320C6000 Chip Support Library API User's Guide for further   */
/* information.                                                    */
/*******************************************************************/

/* Chip definition - Please change this accordingly */
#define CHIP_6713 1

/* Include files */
#include <csl.h>   
#include <csl_mcbsp.h>
#include <csl_edma.h> 
#include <csl_dma.h>
#include <csl_irq.h> 
#include <stdio.h>
#include <string.h>
#include <dsk6713.h>

typedef unsigned int Uint32;

/* Create buffers and aligning them on an L2 cache line boundary. */
#pragma DATA_SECTION(xmitbuf,"xmit_buf");
unsigned short xmitbuf[0x0400];

#pragma DATA_SECTION(recvbuf,"recv_buf");
unsigned short recvbuf[0x0400];

/* Definitions  */
#define BUFFER_SIZE 3              /* total number of UART data words  */
#define TRUE 1
#define FALSE 0

/* Declare CSL objects */
MCBSP_Handle hMcbsp0;           /* handle for McBSP1  */

EDMA_Handle hEdma12;            /* handle for EDMA 14 */
EDMA_Handle hEdma13;            /* handle for EDMA 15 */

/* Global Variables */
volatile int receive_done = FALSE;
volatile int transmit_done = TRUE;
char xmit_msg[BUFFER_SIZE+1] = "\0";
char recv_msg[BUFFER_SIZE+1] = "C";

/* Include the vector table to call the IRQ ISRs hookup */
extern far void vectors();

/* Prototypes   */
void ConfigMcBSP(void);
void ConfigEDMA(void);
char *ProcessReceiveData(void);
short VoteLogic(unsigned short);
interrupt void c_int08(void);

char message[] = "String received: %c (%x)\n";

/*******************************************************************/
/* void main(void)                                                 */
/*******************************************************************/
void main(void)
{
    memset(recv_msg, 0, BUFFER_SIZE+1);
    // Allow board level communication
    DSK6713_init();

    DSK6713_rset(DSK6713_MISC, 1);

    /* initialize the CSL library */
    CSL_init();

    /* enable NMI and GI */
    IRQ_nmiEnable();
    IRQ_globalEnable();

    /* point to the IRQ vector table */
    IRQ_setVecs(vectors);


    /* disable and clear the event interrupt */
    IRQ_reset(IRQ_EVT_EDMAINT);

    /* clear Parameter RAM of EDMA */
    EDMA_clearPram(0x00000000);


    /* process transmit data */
    //ProcessTransmitData();



    /* Open the EDMA channels - EDMA 12 for transmit, EDMA 13 for receive   */
    hEdma12 = EDMA_open(EDMA_CHA_XEVT0, EDMA_OPEN_RESET);
    hEdma13 = EDMA_open(EDMA_CHA_REVT0, EDMA_OPEN_RESET);

    /* Open the McBSP channel 1 */
    hMcbsp0 = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);


    /* Configure the EDMA channels */
    ConfigEDMA();

    /* enable EDMA-CPU interrupt tied to McBSP */
    IRQ_enable(IRQ_EVT_EDMAINT);

    /* enable EDMA channel interrupt to CPU */
    EDMA_intEnable(13);

    /* Enable EDMA channels */
    EDMA_enableChannel(hEdma13);

    /* Setup for McBSP */
    ConfigMcBSP();

    /* Start Sample Rate Generator: set /GRST = 1 */
    MCBSP_enableSrgr(hMcbsp0);

    int waittime;
    /* inserted wait time for McBSP to get ready */
    for (waittime=0; waittime<0xF; waittime++);

    /* Wake up the McBSP as transmitter and receiver */
    MCBSP_enableRcv(hMcbsp0);
    MCBSP_enableXmt(hMcbsp0);

    /* Enable Frame Sync Generator for McBSP 1: set /FRST = 1 */
    //MCBSP_enableFsync(hMcbsp0);

    /* To flag an interrupt to the CPU when EDMA transfer/receive is done */
    unsigned int nextTable = EDMA_getTableAddress(hEdma13);
    while (!receive_done || !transmit_done);

    /* process received data */
    ///char recv_char = ProcessReceiveData();
    //printf(message, recv_char, recv_char);

    IRQ_disable(IRQ_EVT_EDMAINT);
    EDMA_RSET(CIER, 0x0);


    MCBSP_close(hMcbsp0);   /* close McBSP 1 */

    EDMA_close(hEdma13);    /* close EDMA 15 */

}   /* End of main() */
   

/*******************************************************************/
/* void ConfigEDMA(void): set up EDMA channel 14/15 for UART Xmit  */
/*******************************************************************/
void ConfigEDMA(void)
{
    // A table where to store the EDMA paramaters
    EDMA_Handle recvEdmaTable = EDMA_allocTable(EDMA_ALLOC_ANY);

    // The EDMA config
    EDMA_Config recvEdmaConfig = {
        EDMA_OPT_RMK(               // OPT
            EDMA_OPT_PRI_HIGH,      /* 1  */
            EDMA_OPT_ESIZE_16BIT,   /* 01 */
            EDMA_OPT_2DS_NO,        /* 0  */
            EDMA_OPT_SUM_NONE,      /* 00 */
            EDMA_OPT_2DD_NO,        /* 0  */
            EDMA_OPT_DUM_INC,       /* 01 */
            EDMA_OPT_TCINT_YES,     /* 1  */
            EDMA_OPT_TCC_OF(13),    /* 13 */
            EDMA_OPT_LINK_YES,      /* 1  */
            EDMA_OPT_FS_NO          /* 0  */
        ),
        EDMA_SRC_DEFAULT,
        EDMA_CNT_RMK(0, BUFFER_SIZE * 9),          // CNT
        EDMA_DST_DEFAULT,
        EDMA_IDX_RMK(0,0),
        EDMA_RLD_DEFAULT
    };

    recvEdmaConfig.src = MCBSP_getRcvAddr(hMcbsp0);     // SRC
    recvEdmaConfig.dst = (Uint32) recvbuf;              // DST
    recvEdmaConfig.rld =
        // ELERLD
        (BUFFER_SIZE * 9) << 16
        // Link to itself
        | (EDMA_getTableAddress(recvEdmaTable) & 0x0000FFFF);

    EDMA_config(recvEdmaTable, &recvEdmaConfig);
    EDMA_config(hEdma13, &recvEdmaConfig);

} /* End of ConfigEDMA() */



/*******************************************************************/
/* void ConfigMcBSP(void): Setup for McBSP Configuration           */
/*******************************************************************/
void ConfigMcBSP(void)
{

    MCBSP_Config mcbspCfg1 = {

        /* SPCR Setup */
        MCBSP_SPCR_RMK(
            MCBSP_SPCR_FREE_YES,            /* 1  */
            MCBSP_SPCR_SOFT_DEFAULT,        /* 0  */
            MCBSP_SPCR_FRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_GRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_XINTM_XRDY,          /* 00 */
            MCBSP_SPCR_XSYNCERR_DEFAULT,            /* 0  */
            MCBSP_SPCR_XRST_DEFAULT,        /* 0  */
            MCBSP_SPCR_DLB_OFF,         /* 0  */
            MCBSP_SPCR_RJUST_RZF,           /* 00 */
            MCBSP_SPCR_CLKSTP_DISABLE,      /* 0  */
            MCBSP_SPCR_DXENA_OFF,           /* 0  */
            MCBSP_SPCR_RINTM_RRDY,          /* 00 */
            MCBSP_SPCR_RSYNCERR_DEFAULT,            /* 0  */
            MCBSP_SPCR_RRST_DEFAULT         /* 0  */
        ),

        /* RCR Setup */
        MCBSP_RCR_RMK(
            MCBSP_RCR_RPHASE_SINGLE,          /* 1     */
            MCBSP_RCR_RFRLEN2_OF(1),        /* 00010 */
            MCBSP_RCR_RWDLEN2_DEFAULT,         /* 000   */
            MCBSP_RCR_RCOMPAND_MSB,         /* 00    */
            MCBSP_RCR_RFIG_YES,         /* 1     */
            MCBSP_RCR_RDATDLY_1BIT,         /* 01    */
            MCBSP_RCR_RFRLEN1_OF(8),        /* 01000 */
            MCBSP_RCR_RWDLEN1_16BIT,        /* 010   */
            MCBSP_RCR_RWDREVRS_DISABLE      /* 0     */
        ),

        /* XCR Setup */
        MCBSP_XCR_RMK(
            MCBSP_XCR_XPHASE_DUAL,          /* 1     */
            MCBSP_XCR_XFRLEN2_OF(1),        /* 00010 */
            MCBSP_XCR_XWDLEN2_8BIT,         /* 000   */
            MCBSP_XCR_XCOMPAND_MSB,         /* 00    */
            MCBSP_XCR_XFIG_YES,         /* 1     */
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
            // clockDividerValue = roundf(CLK_SRC / ((float)baud * 16.0f)) - 1;
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
    };

    MCBSP_config(hMcbsp0, &mcbspCfg1);

}   /* end of Config_McBSP(void) */




/*******************************************************************/
/* void ProcessReceiveData(void)                                   */
/*                                                                 */
/* This function decodes the data in the receive buffer, "recvbuf" */
/* and strips the framing start (0x0000) and Stop (0xFFFF) words.  */
/* It calls the subroutine VoteLogic() to determine each bit of    */
/* the ASCII character.  It then puts the result in recv_msg.      */
/*******************************************************************/
char *ProcessReceiveData(void)
{
    int i = 0;
    unsigned char recv_char = 0;
    short cnt = -1;
    short recv_val;
    unsigned short  *recvbufptr;    /*receive buffer pointer*/

    /* Point to the receive buffer  */
    recvbufptr  = (unsigned short *)recvbuf;

    /* Process all data in the Receive buffer   */
    recv_char = 0;


    /* Process all data in the Receive buffer   */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        recv_char = 0;

        /* Process each UART frame  */
        for (cnt = -1; cnt < 8; cnt++)
        {
            if(cnt != -1)
            {
                /* get the value of the majority of the bits    */
                recv_val    =   VoteLogic(*recvbufptr);

                /* put received bit into proper place   */
                recv_char   += recv_val << cnt;
            }

            recvbufptr++;
        }   /* end for cnt  */

        /* A full BYTE is decoded. Put in result: recv_msg[i]   */
        recv_msg[i] =   recv_char;
    }
    /* A full BYTE is decoded. Put in result: recv_msg[i]   */
    return   recv_msg;


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


/*******************************************************************/
/* EDMA Data Transfer Completion ISRs                              */
/*******************************************************************/

char expected[]= "%s\n";

void nonono(){
    char *recv_char = ProcessReceiveData();
    printf(expected, recv_char);
}

interrupt void c_int08(void)
{
    if (EDMA_intTest(12))
    {
        EDMA_intClear(12);
        transmit_done = TRUE;
    }

    if (EDMA_intTest(13))
    {
        EDMA_intClear(13);
        nonono();
    }
}
