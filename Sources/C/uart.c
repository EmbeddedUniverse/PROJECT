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
#include <dsk6713.h>

typedef unsigned int Uint32;

/* Create buffers and aligning them on an L2 cache line boundary. */
#pragma DATA_SECTION(xmitbuf,"xmit_buf");
unsigned short xmitbuf[0x0400];

#pragma DATA_SECTION(recvbuf,"recv_buf");
unsigned short recvbuf[0x0400];

/* Definitions  */
#define BUFFER_SIZE 27              /* total number of UART data words  */
#define TRUE 1
#define FALSE 0

/* Declare CSL objects */
MCBSP_Handle hMcbsp0;           /* handle for McBSP1  */
#if (EDMA_SUPPORT)
    EDMA_Handle hEdma12;            /* handle for EDMA 14 */
    EDMA_Handle hEdma13;            /* handle for EDMA 15 */
#endif
#if (DMA_SUPPORT)
    DMA_Handle hDma1;               /* handle for DMA 1 */
    DMA_Handle hDma2;               /* handle for DMA 2 */
#endif

/* Global Variables */
volatile int receive_done = FALSE;
volatile int transmit_done = TRUE;
char xmit_msg[BUFFER_SIZE] = "McBSP does UART on C6000!\n";
char recv_msg[BUFFER_SIZE] = "Transmission didn't work!\n"; 

/* Include the vector table to call the IRQ ISRs hookup */
extern far void vectors();

/* Prototypes   */
void ConfigMcBSP(void);
void ConfigEDMA(void);
void ConfigDMA(void);
void ProcessTransmitData(void);
void ProcessReceiveData(void);
short VoteLogic(unsigned short);
int CheckTestCase(void);
interrupt void c_int11(void);
interrupt void c_int09(void);
interrupt void c_int08(void);


/*******************************************************************/
/* void main(void)                                                 */
/*******************************************************************/
void main(void)
{

    int waittime = 0;
    int works = FALSE;

    DSK6713_init();

    //DSK6713_rset(DSK6713_MISC, 3);

    /* initialize the CSL library */
    CSL_init();

    /* enable NMI and GI */
    IRQ_nmiEnable();
    IRQ_globalEnable();

    /* point to the IRQ vector table */
    IRQ_setVecs(vectors);

    // Allow board level communication


    #if (EDMA_SUPPORT)

        /* disable and clear the event interrupt */
        IRQ_reset(IRQ_EVT_EDMAINT);

        /* clear Parameter RAM of EDMA */
        EDMA_clearPram(0x00000000);

    #endif

    #if (DMA_SUPPORT)

        DMA_reset(INV);

    #endif

    /* process transmit data */
    printf("Processing Transmit string...\n");
    ProcessTransmitData();
    printf("String transmitted: %s \n", xmit_msg);

    #if (EDMA_SUPPORT)

        /* Open the EDMA channels - EDMA 14 for transmit, EDMA 15 for receive   */
        hEdma12 = EDMA_open(EDMA_CHA_XEVT0, EDMA_OPEN_RESET);
        hEdma13 = EDMA_open(EDMA_CHA_REVT0, EDMA_OPEN_RESET);

    #endif

    #if (DMA_SUPPORT)

        /* Open the DMA channels - DMA 1 for transmit, DMA 2 for receive */
        hDma1 = DMA_open(DMA_CHA0, DMA_OPEN_RESET);
        hDma2 = DMA_open(DMA_CHA0, DMA_OPEN_RESET);

    #endif

    /* Open the McBSP channel 1 */
    hMcbsp0 = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);

    #if (EDMA_SUPPORT)

        /* Configure the EDMA channels */
        ConfigEDMA();

        /* enable EDMA-CPU interrupt tied to McBSP */
        IRQ_enable(IRQ_EVT_EDMAINT);

        /* enable EDMA channel interrupt to CPU */
        EDMA_intEnable(12);
        EDMA_intEnable(13);

        /* Enable EDMA channels */
        EDMA_enableChannel(hEdma12);
        EDMA_enableChannel(hEdma13);

    #endif

    #if (DMA_SUPPORT)

        /* Configure the DMA channels */
        ConfigDMA();

        IRQ_disable(IRQ_EVT_DMAINT1);
        IRQ_disable(IRQ_EVT_DMAINT2);

        IRQ_clear(IRQ_EVT_DMAINT1);
        IRQ_clear(IRQ_EVT_DMAINT2);

        IRQ_enable(IRQ_EVT_DMAINT1);
        IRQ_enable(IRQ_EVT_DMAINT2);

        DMA_start(hDma1);            /*start DMA channel 1*/
        DMA_start(hDma2);            /*start DMA channel 2*/

    #endif

    /* Setup for McBSP */
    ConfigMcBSP();

    /* Start Sample Rate Generator: set /GRST = 1 */
    MCBSP_enableSrgr(hMcbsp0);

    /* inserted wait time for McBSP to get ready */
    for (waittime=0; waittime<0xF; waittime++);

    /* Wake up the McBSP as transmitter and receiver */
    MCBSP_enableRcv(hMcbsp0);
    MCBSP_enableXmt(hMcbsp0);

    /* Enable Frame Sync Generator for McBSP 1: set /FRST = 1 */
    MCBSP_enableFsync(hMcbsp0);

    /* To flag an interrupt to the CPU when EDMA transfer/receive is done */
    while (!receive_done || !transmit_done);

    /* Check to make sure the test case works */
    works = CheckTestCase();
    if (works != 0) printf("Transmission Error....\n\n");
    else printf("Received data matched transmitted data!\n\n");

    /* process received data */
    printf("Processing Receive string...\n");
    ProcessReceiveData();
    printf("String received: %s \n", recv_msg);

    #if (EDMA_SUPPORT)

        IRQ_disable(IRQ_EVT_EDMAINT);
        EDMA_RSET(CIER, 0x0);

    #endif

    #if (DMA_SUPPORT)

        IRQ_disable(IRQ_EVT_DMAINT1);
        IRQ_disable(IRQ_EVT_DMAINT2);

    #endif

    MCBSP_close(hMcbsp0);   /* close McBSP 1 */

    #if (EDMA_SUPPORT)

        EDMA_close(hEdma12);    /* close EDMA 14 */
        EDMA_close(hEdma13);    /* close EDMA 15 */

    #endif

    #if (DMA_SUPPORT)

        DMA_close(hDma1);   /* close DMA 1 */
        DMA_close(hDma2);   /* close DMA 2 */

    #endif
}   /* End of main() */
   

/*******************************************************************/
/* void ConfigEDMA(void): set up EDMA channel 14/15 for UART Xmit  */
/*******************************************************************/
#if (EDMA_SUPPORT)

void ConfigEDMA(void)
{
    EDMA_configArgs(hEdma12,

        /* OPT Setup */
        #if (C64_SUPPORT)
            EDMA_OPT_RMK(
            EDMA_OPT_PRI_HIGH,      /* 1  */
            EDMA_OPT_ESIZE_16BIT,           /* 01 */
                EDMA_OPT_2DS_NO,        /* 0  */
                EDMA_OPT_SUM_INC,       /* 01 */
                EDMA_OPT_2DD_NO,        /* 0  */
                EDMA_OPT_DUM_NONE,      /* 00 */
                EDMA_OPT_TCINT_YES,     /* 1  */
                EDMA_OPT_TCC_OF(14),            /* 14 */
            EDMA_OPT_TCCM_DEFAULT,          /* 0  */
            EDMA_OPT_ATCINT_DEFAULT,        /* 0  */
            EDMA_OPT_ATCC_DEFAULT,          /* 0  */
            EDMA_OPT_PDTS_DEFAULT,          /* 0  */
            EDMA_OPT_PDTD_DEFAULT,          /* 0  */
            EDMA_OPT_LINK_NO,       /* 0  */
                EDMA_OPT_FS_NO          /* 0  */
            ),
        #else
            EDMA_OPT_RMK(
            EDMA_OPT_PRI_HIGH,      /* 1  */
            EDMA_OPT_ESIZE_16BIT,           /* 01 */
                EDMA_OPT_2DS_NO,        /* 0  */
                EDMA_OPT_SUM_INC,       /* 01 */
                EDMA_OPT_2DD_NO,        /* 0  */
                EDMA_OPT_DUM_NONE,      /* 00 */
                EDMA_OPT_TCINT_YES,     /* 1  */
                EDMA_OPT_TCC_OF(14),            /* 14 */
                EDMA_OPT_LINK_NO,       /* 0  */
                EDMA_OPT_FS_NO          /* 0  */
            ),
        #endif

        /* SRC Setup */
        EDMA_SRC_RMK((Uint32) xmitbuf),        /*xmitbuf address*/

        /* CNT Setup */
        EDMA_CNT_RMK(
            EDMA_CNT_FRMCNT_DEFAULT,
            EDMA_CNT_ELECNT_OF(BUFFER_SIZE*11)
        ),

        /* DST Setup */
        EDMA_DST_RMK(MCBSP_getXmtAddr(hMcbsp0)),

        /* IDX Setup */
        EDMA_IDX_RMK(0,0),

        /* RLD Setup */
        EDMA_RLD_RMK(0,0)
        );

    EDMA_configArgs(hEdma13,

        /* OPT Setup */
        #if (C64_SUPPORT)
            EDMA_OPT_RMK(
            EDMA_OPT_PRI_HIGH,      /* 1  */
            EDMA_OPT_ESIZE_16BIT,           /* 01 */
                EDMA_OPT_2DS_NO,        /* 0  */
                EDMA_OPT_SUM_NONE,      /* 00 */
                EDMA_OPT_2DD_NO,        /* 0  */
                EDMA_OPT_DUM_INC,       /* 01 */
                EDMA_OPT_TCINT_YES,     /* 1  */
                EDMA_OPT_TCC_OF(15),            /* 15 */
            EDMA_OPT_TCCM_DEFAULT,          /* 0  */
            EDMA_OPT_ATCINT_DEFAULT,        /* 0  */
            EDMA_OPT_ATCC_DEFAULT,          /* 0  */
            EDMA_OPT_PDTS_DEFAULT,          /* 0  */
            EDMA_OPT_PDTD_DEFAULT,          /* 0  */
                EDMA_OPT_LINK_NO,       /* 0  */
                EDMA_OPT_FS_NO          /* 0  */
            ),
        #else
            EDMA_OPT_RMK(
            EDMA_OPT_PRI_HIGH,          /* 1  */
            EDMA_OPT_ESIZE_16BIT,           /* 01 */
                EDMA_OPT_2DS_NO,        /* 0  */
                EDMA_OPT_SUM_NONE,      /* 00 */
                EDMA_OPT_2DD_NO,        /* 0  */
                EDMA_OPT_DUM_INC,       /* 01 */
                EDMA_OPT_TCINT_YES,     /* 1  */
                EDMA_OPT_TCC_OF(15),            /* 15 */
                EDMA_OPT_LINK_NO,       /* 0  */
                EDMA_OPT_FS_NO          /* 0  */
            ),
        #endif

        /* SRC Setup */
        EDMA_SRC_RMK(MCBSP_getRcvAddr(hMcbsp0)),

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

#endif


/*******************************************************************/
/* void ConfigDMA(void): set up DMA channels 1 & 2 for UART Xmit   */
/*******************************************************************/
#if (DMA_SUPPORT)

void ConfigDMA(void)
{

    DMA_configArgs(hDma1,

        /* PRICTL Setup */
        DMA_PRICTL_RMK(
            DMA_PRICTL_DSTRLD_NONE,
            DMA_PRICTL_SRCRLD_NONE,
            DMA_PRICTL_EMOD_HALT,
            DMA_PRICTL_FS_DISABLE,
            DMA_PRICTL_TCINT_ENABLE,
            DMA_PRICTL_PRI_DMA,
            DMA_PRICTL_WSYNC_XEVT1,
            DMA_PRICTL_RSYNC_NONE,
            DMA_PRICTL_INDEX_NA,
            DMA_PRICTL_CNTRLD_NA,
            DMA_PRICTL_SPLIT_DISABLE,
            DMA_PRICTL_ESIZE_16BIT,
            DMA_PRICTL_DSTDIR_NONE,
            DMA_PRICTL_SRCDIR_INC,
            DMA_PRICTL_START_STOP
        ),

        /* SECCTL Setup */
        DMA_SECCTL_RMK(
            DMA_SECCTL_WSPOL_ACTIVEHIGH,
            DMA_SECCTL_RSPOL_ACTIVEHIGH,
            DMA_SECCTL_FSIG_NORMAL,
            DMA_SECCTL_DMACEN_FRAMECOND,
            DMA_SECCTL_WSYNCCLR_NOTHING,
            DMA_SECCTL_WSYNCSTAT_CLEAR,
            DMA_SECCTL_RSYNCCLR_NOTHING,
            DMA_SECCTL_RSYNCSTAT_CLEAR,
            DMA_SECCTL_WDROPIE_DISABLE,
            DMA_SECCTL_WDROPCOND_CLEAR,
            DMA_SECCTL_RDROPIE_DISABLE,
            DMA_SECCTL_RDROPCOND_CLEAR,
            DMA_SECCTL_BLOCKIE_ENABLE,      /* BLOCK IE=1: enables DMA channel int */
            DMA_SECCTL_BLOCKCOND_CLEAR,
            DMA_SECCTL_LASTIE_DISABLE,
            DMA_SECCTL_LASTCOND_CLEAR,
            DMA_SECCTL_FRAMEIE_DISABLE,
            DMA_SECCTL_FRAMECOND_CLEAR,
            DMA_SECCTL_SXIE_DISABLE,
            DMA_SECCTL_SXCOND_CLEAR
        ),

        /* SRC Setup */
        DMA_SRC_RMK((Uint32) xmitbuf),              /*xmitbuf*/

        /* DST Setup */
        DMA_DST_RMK(MCBSP_getXmtAddr(hMcbsp0)),     /*McBSP DXR */

        /* XFRCNT Setup */
        DMA_XFRCNT_RMK(
            DMA_XFRCNT_FRMCNT_OF(1),
            DMA_XFRCNT_ELECNT_OF(BUFFER_SIZE*11)
        )

    );

    DMA_configArgs(hDma2,

        /* PRICTL Setup */
        DMA_PRICTL_RMK(
            DMA_PRICTL_DSTRLD_NONE,
            DMA_PRICTL_SRCRLD_NONE,
            DMA_PRICTL_EMOD_HALT,
            DMA_PRICTL_FS_DISABLE,
            DMA_PRICTL_TCINT_ENABLE,
            DMA_PRICTL_PRI_DMA,
            DMA_PRICTL_WSYNC_NONE,
            DMA_PRICTL_RSYNC_REVT1,
            DMA_PRICTL_INDEX_NA,
            DMA_PRICTL_CNTRLD_NA,
            DMA_PRICTL_SPLIT_DISABLE,
            DMA_PRICTL_ESIZE_16BIT,
            DMA_PRICTL_DSTDIR_INC,
            DMA_PRICTL_SRCDIR_NONE,
            DMA_PRICTL_START_STOP
        ),

        /* SECCTL Setup */
        DMA_SECCTL_RMK(
            DMA_SECCTL_WSPOL_ACTIVEHIGH,
            DMA_SECCTL_RSPOL_ACTIVEHIGH,
            DMA_SECCTL_FSIG_NORMAL,
            DMA_SECCTL_DMACEN_FRAMECOND,
            DMA_SECCTL_WSYNCCLR_NOTHING,
            DMA_SECCTL_WSYNCSTAT_CLEAR,
            DMA_SECCTL_RSYNCCLR_NOTHING,
            DMA_SECCTL_RSYNCSTAT_CLEAR,
            DMA_SECCTL_WDROPIE_DISABLE,
            DMA_SECCTL_WDROPCOND_CLEAR,
            DMA_SECCTL_RDROPIE_DISABLE,
            DMA_SECCTL_RDROPCOND_CLEAR,
            DMA_SECCTL_BLOCKIE_ENABLE,      /* BLOCK IE=1: enables DMA channel int */
            DMA_SECCTL_BLOCKCOND_CLEAR,
            DMA_SECCTL_LASTIE_DISABLE,
            DMA_SECCTL_LASTCOND_CLEAR,
            DMA_SECCTL_FRAMEIE_DISABLE,
            DMA_SECCTL_FRAMECOND_CLEAR,
            DMA_SECCTL_SXIE_DISABLE,
            DMA_SECCTL_SXCOND_CLEAR
        ),

        /* SRC Setup */
        DMA_SRC_RMK(MCBSP_getRcvAddr(hMcbsp0)),     /*McBSP DRR */

        /* DST Setup */
        DMA_DST_RMK((Uint32) recvbuf),              /*recvbuf*/

        /* XFRCNT Setup */
        DMA_XFRCNT_RMK(
            DMA_XFRCNT_FRMCNT_OF(1),
            DMA_XFRCNT_ELECNT_OF(BUFFER_SIZE*11)
        )

    );

} /* End of ConfigDMA() */

#endif

/*******************************************************************/
/* void ConfigMcBSP(void): Setup for McBSP Configuration           */
/*******************************************************************/
void ConfigMcBSP(void)
{

    MCBSP_Config mcbspCfg1 = {

        /* SPCR Setup */
        #if (DMA_SUPPORT)
            MCBSP_SPCR_RMK(
                MCBSP_SPCR_FRST_DEFAULT,        /* 0  */
                MCBSP_SPCR_GRST_DEFAULT,        /* 0  */
                MCBSP_SPCR_XINTM_XRDY,          /* 00 */
                MCBSP_SPCR_XSYNCERR_DEFAULT,            /* 0  */
                MCBSP_SPCR_XRST_DEFAULT,        /* 0  */
                MCBSP_SPCR_DLB_OFF,         /* 0  */
                MCBSP_SPCR_RJUST_RZF,           /* 00 */
                MCBSP_SPCR_CLKSTP_DISABLE,      /* 0x */
                MCBSP_SPCR_RINTM_RRDY,          /* 00 */
                MCBSP_SPCR_RSYNCERR_DEFAULT,            /* 0  */
                MCBSP_SPCR_RRST_DEFAULT         /* 0  */
            ),
        #endif
        #if (EDMA_SUPPORT)
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
        #endif

        /* RCR Setup */
        #if (DMA_SUPPORT)
            MCBSP_RCR_RMK(
            MCBSP_RCR_RPHASE_DUAL,          /* 1     */
            MCBSP_RCR_RFRLEN2_OF(1),        /* 00010 */
                MCBSP_RCR_RWDLEN2_8BIT,         /* 000   */
                MCBSP_RCR_RCOMPAND_MSB,         /* 00    */
                MCBSP_RCR_RFIG_YES,         /* 1     */
                MCBSP_RCR_RDATDLY_1BIT,         /* 01    */
                MCBSP_RCR_RFRLEN1_OF(8),        /* 01000 */
                MCBSP_RCR_RWDLEN1_16BIT         /* 010   */
            ),
        #endif
        #if (EDMA_SUPPORT)
            MCBSP_RCR_RMK(
            MCBSP_RCR_RPHASE_DUAL,          /* 1     */
            MCBSP_RCR_RFRLEN2_OF(1),        /* 00010 */
                MCBSP_RCR_RWDLEN2_8BIT,         /* 000   */
                MCBSP_RCR_RCOMPAND_MSB,         /* 00    */
                MCBSP_RCR_RFIG_YES,         /* 1     */
                MCBSP_RCR_RDATDLY_1BIT,         /* 01    */
                MCBSP_RCR_RFRLEN1_OF(8),        /* 01000 */
                MCBSP_RCR_RWDLEN1_16BIT,        /* 010   */
                MCBSP_RCR_RWDREVRS_DISABLE      /* 0     */
            ),
        #endif

        /* XCR Setup */
        #if (DMA_SUPPORT)
            MCBSP_XCR_RMK(
                MCBSP_XCR_XPHASE_DUAL,          /* 1     */
                MCBSP_XCR_XFRLEN2_OF(1),        /* 00010 */
            MCBSP_XCR_XWDLEN2_8BIT,         /* 000   */
            MCBSP_XCR_XCOMPAND_MSB,         /* 00    */
            MCBSP_XCR_XFIG_YES,         /* 1     */
            MCBSP_XCR_XDATDLY_0BIT,         /* 00    */
            MCBSP_XCR_XFRLEN1_OF(8),        /* 01000 */
            MCBSP_XCR_XWDLEN1_16BIT         /* 010   */
            ),
        #endif
        #if (EDMA_SUPPORT)
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
        #endif

        /* SRGR Setup */
        MCBSP_SRGR_RMK(
            MCBSP_SRGR_GSYNC_FREE,              /* 0      */
            MCBSP_SRGR_CLKSP_RISING,            /* 0      */
            MCBSP_SRGR_CLKSM_INTERNAL,          /* 1      */
            MCBSP_SRGR_FSGM_DXR2XSR,            /* 0      */
            MCBSP_SRGR_FPER_DEFAULT,            /* 0      */
            MCBSP_SRGR_FWID_DEFAULT,            /* 0      */
 //         MCBSP_SRGR_CLKGDV_OF(40)            /* CLKGDV */
            MCBSP_SRGR_CLKGDV_OF(108)           /* CLKGDV */
        ),

        /* MCR Setup */
        MCBSP_MCR_DEFAULT,                      /* default values */

        /* RCER Setup */
        #if (C64_SUPPORT)
            MCBSP_RCERE0_DEFAULT,
            MCBSP_RCERE1_DEFAULT,
            MCBSP_RCERE2_DEFAULT,
            MCBSP_RCERE3_DEFAULT,
        #else
            MCBSP_RCER_DEFAULT,                     /* default values */
        #endif

        /* XCER Setup */
        #if (C64_SUPPORT)
            MCBSP_XCERE0_DEFAULT,
            MCBSP_XCERE1_DEFAULT,
            MCBSP_XCERE2_DEFAULT,
            MCBSP_XCERE3_DEFAULT,
        #else
            MCBSP_XCER_DEFAULT,                     /* default values */
        #endif

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
/* void ProcessTransmitData(void)                                  */
/*                                                                 */
/* This function expands each of the 8-bit ASCII characters in the */
/* transmit string "xmit_msg" into UART transmission 16-bit word   */
/* and place them in the transmit buffer "xmitbuf".  In addition,  */
/* 16-bit Start and 8-bit Stop framing words, respectively, are    */
/* inserted before and after each of the ASCII characters in the   */
/* buffer.                                                         */
/*******************************************************************/
void ProcessTransmitData(void)
{
    int     i;
    short   cnt = 1;
    unsigned char   xmit_char;
    unsigned short  *xmitbufptr;

    /* point to Transmit Buffer */
    xmitbufptr = (unsigned short *)xmitbuf;

    for (i=0; i<(sizeof(xmitbuf)/sizeof(unsigned int)); i++)
    {
        xmitbufptr[i] = 0x0000; /* zero fill buffer */
    }

    xmitbufptr = (unsigned short *)xmitbuf;

    /* Process data BYTES in xmit_msg[] and put in xmit buffer  */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        /*Get transmit character (one byte) from xmit_msg[] and put in xmit buffer*/
        xmit_char   =   xmit_msg[i];

        /* Process each BYTE of transmit character  */
        for (cnt = -1; cnt < 10; cnt++)
        {
            if (cnt == -1)
                *xmitbufptr++   =   0x0000;

            else if (cnt == 8 || cnt ==9)
                *xmitbufptr++   =   0xFFFF;

            else if (xmit_char & (1 << cnt))
                *xmitbufptr++   =   0xFFFF;

            else
                *xmitbufptr++   =   0x0000;

        }   /* end for cnt  */

    }   /* end for i    */

}   /* end ProcessTransmitData  */


/*******************************************************************/
/* void ProcessReceiveData(void)                                   */
/*                                                                 */
/* This function decodes the data in the receive buffer, "recvbuf" */
/* and strips the framing start (0x0000) and Stop (0xFFFF) words.  */
/* It calls the subroutine VoteLogic() to determine each bit of    */
/* the ASCII character.  It then puts the result in recv_msg.      */
/*******************************************************************/
void ProcessReceiveData(void)
{
    int i;
    unsigned char recv_char = 0;
    short cnt = -1;
    short recv_val;
    unsigned short  raw_data;
    unsigned short  *recvbufptr;    /*receive buffer pointer*/

    /* Point to the receive buffer  */
    recvbufptr  = (unsigned short *)recvbuf;

    /* Process all data in the Receive buffer   */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        recv_char = 0;

        /* Process each UART frame  */
        for (cnt = -1; cnt < 10; cnt++)
        {
            if(cnt == -1 || cnt == 8 || cnt == 9)
            {
                /* Ignore Start and Stop bits   */
                *recvbufptr++;
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
        recv_msg[i] =   recv_char;

    }   /* end for i    */

}   /* end ProcessReceiveData() function    */


/*******************************************************************/
/* void CheckTestCase(void)                                        */
/*******************************************************************/
int CheckTestCase(void)
{
    unsigned short *source;
    unsigned short *result;
    unsigned int i = 0;
    short cnt = -1;
    int error = 0;

    source = (unsigned short *) xmitbuf;
    result = (unsigned short *) recvbuf;

    for (i = 0; i < BUFFER_SIZE ; i++)
    {
        for (cnt = -1; cnt < 10; cnt++)
        {
            /* Ignore the start and stop bits */
            if(cnt == -1 || cnt == 8 || cnt ==9)
            {
                source++;
                result++;
            }
            else
            {
                if (*source != *result)
                {
                    error = i + 1;
                    break;
                }
                source++;
                result++;
            }
        }
    }

    return(error);

}   /* end CheckTestCase() function */


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


interrupt void c_int11(void)  /* DMA 2 */
{
    #if (DMA_SUPPORT)
        transmit_done = TRUE;
        printf("Transmit Completed\n");
    #endif
}

interrupt void c_int09(void)  /* DMA 1 */
{
    #if (DMA_SUPPORT)
        receive_done = TRUE;
        printf("Receive Completed\n");
    #endif
}


interrupt void c_int08(void)
{
    #if (EDMA_SUPPORT)
        if (EDMA_intTest(14))
        {
            EDMA_intClear(14);
            transmit_done = TRUE;
            printf("Transmit Completed\n");
        }

        if (EDMA_intTest(15))
        {
            EDMA_intClear(15);
            receive_done = TRUE;
            printf("Receive Completed\n\n");
        }
    #endif
}
