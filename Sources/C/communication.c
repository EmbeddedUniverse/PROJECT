#include <dsk6713.h>
#include "communication.h"
#include "max3111.h"

COM_init()
{
    int error = 0;

    DSK6713_init();
    SPI_init();

    COM_selectInterface(ACCEL);
    if (MAX3111_init(BAUD_57600) != 0)
    {
        printf("Error Setting up MAX3111_%d\n", (int)ACCEL);
        error = 1;
    }

    COM_selectInterface(PIC);
    if (MAX3111_init(BAUD_9600) != 0)
    {
        printf("Error Setting up MAX3111_%d\n", (int)PIC);
        error = 1;
    }

    return error;
}

COM_selectInterface(SPI_Interface interface)
{
    if (interface == 0)
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL1));
    }
    else if (interface == 1)
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL1));
    }
    else
        return 1;


    return 0;
}

