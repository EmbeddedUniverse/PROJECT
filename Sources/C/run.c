/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <stdio.h>
#include <dsk6713.h>
#include "max3111.h"

void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    SPI_init();

    while(1)
    {


        DSK6713_waitusec(1000000);
        if (flagUART)
        {
            printf("Received: %d\n", readByteUART());
            flagUART = false;

        }
        sendByteUART(101);
    }
}

