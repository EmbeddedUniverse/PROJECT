/***************************************************************************
    Include headers :
***************************************************************************/
#include <csl.h>
#include <csl_mcbsp.h>
#include <dsk6713.h>
#include <dsk6713_aic23.h>

/***************************************************************************
    Include Module Header :
***************************************************************************/

#define MAX3111_MODULE_IMPORT
#include "max3111.h"

/****************************************************************************
    Extern content declaration :
****************************************************************************/


/****************************************************************************
    Private macros and constants :
****************************************************************************/



/****************************************************************************
    Private Types :
****************************************************************************/



/****************************************************************************
    Private global variables :
****************************************************************************/

MCBSP_Handle SPI_PortHandle;

MCBSP_Config MCBSP_SPI_Config = {
    MCBSP_FMKS(SPCR, FREE, NO)              |
    MCBSP_FMKS(SPCR, SOFT, NO)              |
    MCBSP_FMKS(SPCR, FRST, YES)             |
    MCBSP_FMKS(SPCR, GRST, YES)             |
    MCBSP_FMKS(SPCR, XINTM, XRDY)           |
    MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
    MCBSP_FMKS(SPCR, XRST, YES)             |
    MCBSP_FMKS(SPCR, DLB, OFF)              |
    MCBSP_FMKS(SPCR, RJUST, RZF)            |
    MCBSP_FMKS(SPCR, CLKSTP, DELAY)         |
    MCBSP_FMKS(SPCR, DXENA, OFF)            |
    MCBSP_FMKS(SPCR, RINTM, RRDY)           |
    MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
    MCBSP_FMKS(SPCR, RRST, YES),

    MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
    MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
    MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
    MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
    MCBSP_FMKS(RCR, RFIG, NO)               |
    MCBSP_FMKS(RCR, RDATDLY, 0BIT)          |
    MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
    MCBSP_FMKS(RCR, RWDLEN1, 16BIT)         | // This changes to 8 bits per frame
    MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

    MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
    MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
    MCBSP_FMKS(XCR, XFIG, NO)               |
    MCBSP_FMKS(XCR, XDATDLY, 0BIT)          |
    MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
    MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         | // This changes to 8 bits per frame
    MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

    MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSM, INTERNAL)       |
    MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
    MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
    MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
    MCBSP_FMKS(SRGR, CLKGDV, OF(64)),           // Min SCLK period is 238 ns -> rounding to 600 ns

    MCBSP_MCR_DEFAULT,
    MCBSP_RCER_DEFAULT,
    MCBSP_XCER_DEFAULT,

    MCBSP_FMKS(PCR, XIOEN, SP)              |
    MCBSP_FMKS(PCR, RIOEN, SP)              |
    MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
    MCBSP_FMKS(PCR, FSRM, INTERNAL)         |
    MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
    MCBSP_FMKS(PCR, CLKRM, OUTPUT)          |
    MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
    MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
    MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
    MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
    MCBSP_FMKS(PCR, CLKXP, RISING)          |
    MCBSP_FMKS(PCR, CLKRP, RISING)
};

const Uint32 SPI_WRITE_CONFIG = 0xC000;
const Uint32 SPI_READ_CONFIG = 0x4000;
const Uint32 SPI_WRITE_DATA = 0x8000;
const Uint32 SPI_READ_DATA = 0x0000;

#define N_FIFO_ENABLE 13
#define SOFT_SHUTDOWN 12
#define N_TRANSMIT_BUFF_EMPTY_INTERRUPT 11
#define N_DATA_AVAILABLE_INTERRUPT 10
#define N_PARITY_BIT_INTERRUPT 9
#define N_FRAMING_ERROR_INTERRUPT 8
#define IrDA_MODE 7
#define TWO_STOP_BITS 6
#define PARITY_ENABLE 5
#define WORD_LENGHT 4
#define UART_BAUD_RATE_DIV 0
#define DOUT 1<<15

const Uint32 MAX3111_Config =
    0   << N_FIFO_ENABLE |
    0   << SOFT_SHUTDOWN |
    0   << N_TRANSMIT_BUFF_EMPTY_INTERRUPT |
    1   << N_DATA_AVAILABLE_INTERRUPT |
    0   << N_PARITY_BIT_INTERRUPT |
    0   << N_FRAMING_ERROR_INTERRUPT |
    0   << IrDA_MODE |
    1   << TWO_STOP_BITS |
    0   << PARITY_ENABLE |
    0   << WORD_LENGHT |
    0   << UART_BAUD_RATE_DIV;

/****************************************************************************
    Private functions :
****************************************************************************/



/****************************************************************************
    Public functions :
****************************************************************************/

void SPI_init()
{
    CSL_init();
    DSK6713_init();

    // Divert McBSP signals to daughter card
    DSK6713_rset(DSK6713_MISC, MCBSP1SEL);

    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);

    SPI_PortHandle = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);

    MCBSP_config(SPI_PortHandle, &MCBSP_SPI_Config);

    MCBSP_enableSrgr(SPI_PortHandle);
    DSK6713_waitusec(10);


    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(SPI_PortHandle);
    MCBSP_enableXmt(SPI_PortHandle);
    DSK6713_waitusec(10);
}

int MAX3111_init(BaudRate baud)
{
    Uint32 settings = MAX3111_Config | baud << UART_BAUD_RATE_DIV;

    MCBSP_write(SPI_PortHandle, SPI_WRITE_CONFIG | settings);
    while(! MCBSP_rrdy(SPI_PortHandle));
    MCBSP_read(SPI_PortHandle);
    DSK6713_waitusec(100);

    // Check if config is applied
    while(! MCBSP_xrdy(SPI_PortHandle));
    MCBSP_write(SPI_PortHandle, SPI_READ_CONFIG);
    while(! MCBSP_rrdy(SPI_PortHandle));
    Uint32 configReturned = MCBSP_read(SPI_PortHandle) & ~SPI_WRITE_CONFIG;

    if (configReturned != settings)
    {
        return 1;
    }

    return 0;
}

// Returns true when there is data to read on the interface
bool sendByteUART(unsigned char data)
{
    while(! MCBSP_xrdy(SPI_PortHandle));
    MCBSP_write(SPI_PortHandle, SPI_WRITE_DATA | data);
    while(! MCBSP_rrdy(SPI_PortHandle));
    return (MCBSP_read(SPI_PortHandle) & DOUT) != 0;
}

unsigned char readByteUART()
{
    while(! MCBSP_xrdy(SPI_PortHandle));
    MCBSP_write(SPI_PortHandle, SPI_READ_DATA);
    while(! MCBSP_rrdy(SPI_PortHandle));
    return MCBSP_read(SPI_PortHandle) & 0xFF;
}


