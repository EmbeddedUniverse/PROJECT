#include <dsk6713.h>
#include "communication.h"


COM_init()
{
    void DSK6713_init();

    COM_selectInterface(DEFAULT);

    return 0;
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

