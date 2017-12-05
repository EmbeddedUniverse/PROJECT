#include <dsk6713.h>
#include "communication.h"
#include "max3111.h"
#include "addresses.h"

extern void vectors();   // Vecteurs d'interruption

volatile bool flagUART;
volatile ReceiveCallBack recvCallBacks[MAX3111_DEVICE_COUNT] = {0,0};


COM_init()
{
    int error = 0;

    CSL_init();
    DSK6713_init();
    SPI_init();
    DSK6713_waitusec(100);

    COM_selectInterface(ACCEL);
    if (MAX3111_init(BAUD_57600) != 0)
    {
        printf("Error Setting up MAX3111_%d\n", (int)ACCEL);
        error = 1;
    }
    else
        clearFIFO(ACCEL);

    COM_selectInterface(PIC);
    if (MAX3111_init(BAUD_9600) != 0)
    {
        printf("Error Setting up MAX3111_%d\n", (int)PIC);
        error = 1;
    }
    else
        clearFIFO(PIC);

    /* point to the IRQ vector table */
    IRQ_setVecs(vectors);
    IRQ_map(IRQ_EVT_EXTINT4, IRQ_EVT_EXTINT4);
    IRQ_map(IRQ_EVT_EXTINT5, IRQ_EVT_EXTINT5);

    IRQ_enable(IRQ_EVT_EXTINT4);
    IRQ_enable(IRQ_EVT_EXTINT5);
    GPPOL = 0xFFFF;
    GPDIR &= ~(1 << GP4DIR);
    GPDIR &= ~(1 << GP5DIR);

    flagUART = false;

    /* enable NMI and GI */
    IRQ_globalEnable();
    IRQ_nmiEnable();

    return error;
}

COM_selectInterface(SPI_Interface interface)
{
    if (interface == 0)
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL0));
    }
    else if (interface == 1)
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL0));
    }
    else
        return 1;

    DSK6713_waitusec(2);

    return 0;
}

COM_send(unsigned char data, SPI_Interface interface)
{
    COM_selectInterface(interface);

    DSK6713_waitusec(1);

    sendByteUART(data);

    return 0;
}

void COM_setReceiveCallBack(void (*callBack)(unsigned char), SPI_Interface interface)
{
    recvCallBacks[(int)interface] = (ReceiveCallBack)callBack;
}

/****************************************************************************
    ISR :
****************************************************************************/

void interrupt ACC_recvRoutine()
{
    COM_selectInterface(ACCEL);
    unsigned char recvByte = readByteUART();

    if (recvCallBacks[ACCEL] != 0)
        recvCallBacks[ACCEL](recvByte);
}

void interrupt PIC_recvRoutine()
{
    COM_selectInterface(ACCEL);
    unsigned char recvByte = readByteUART();

    if (recvCallBacks[PIC] != 0)
        recvCallBacks[PIC](recvByte);
}

void clearFIFO(SPI_Interface interface)
{
    COM_selectInterface(interface);
    flushFIFO();
}
