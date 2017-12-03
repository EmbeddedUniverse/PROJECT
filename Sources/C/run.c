/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <stdio.h>
#include <dsk6713.h>
#include <dsk6713_led.h>
#include "voiceDetector.h"
#include "piouRecognition.h"
#include "communication.h"

void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    COM_init();

    COM_selectInterface(ACCEL);
    while(1)
    {
        COM_selectInterface(ACCEL);
        DSK6713_waitusec(1000000);
        COM_selectInterface(PIC);
        DSK6713_waitusec(1000000);
    }
}

