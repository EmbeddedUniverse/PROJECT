/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <csl.h>
#include <dsk6713.h>
#include "run.h"
#include "serial.h"

void printRes(const char* message, size_t length) {
    printf("%s\r\n", message);
}

void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    /* initialize the CSL and BSL library */
    CSL_init();
    DSK6713_init();


    setupSerial(McBSP0, BAUD_57600, 16, printRes);


    while(1);
}
