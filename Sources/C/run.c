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
#include "circularBuffer.h"
#include "accelProtocol_UART.h"

#define ACC_ANALYSIS_SIZE 128
#define ACC_BUFF_SIZE 2*ACC_ANALYSIS_SIZE

void handleNextReading(const char* message, size_t length);

#pragma DATA_ALIGN(xAccArray, ACC_BUFF_SIZE * sizeof(float))
float xAccArray[ACC_BUFF_SIZE];
circularQBuffer xAccQBuff;

#pragma DATA_ALIGN(yAccArray, ACC_BUFF_SIZE * sizeof(float))
float yAccArray[ACC_BUFF_SIZE];
circularQBuffer yAccQBuff;

#pragma DATA_ALIGN(zAccArray, ACC_BUFF_SIZE * sizeof(float))
float zAccArray[ACC_BUFF_SIZE];
circularQBuffer zAccQBuff;


void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    // Setting up the circular buffer handles
    xAccQBuff = initQBuffer(xAccArray, ACC_BUFF_SIZE);
    yAccQBuff = initQBuffer(yAccArray, ACC_BUFF_SIZE);
    zAccQBuff = initQBuffer(zAccArray, ACC_BUFF_SIZE);

    /* initialize the CSL and BSL library */
    CSL_init();
    DSK6713_init();


    setupSerial(McBSP0, BAUD_57600, FRAME_LENGTH, handleNextReading);


    while(1){
        if (xAccQBuff.quarterFlag)
        {
            //printf("Analysis should start for X\n");
            xAccQBuff.quarterFlag = false;
        }

        if (yAccQBuff.quarterFlag)
        {
            //printf("Analysis should start for Y\n");
            yAccQBuff.quarterFlag = false;
        }

        if (zAccQBuff.quarterFlag)
        {
            //printf("Analysis should start for Z\n");
            zAccQBuff.quarterFlag = false;
        }
    }
}

void handleNextReading(const char* message, size_t length){

    AccelDecodedData readings = convertAccString2Floats(message);

    add2QBuffer(readings.x, &xAccQBuff);
    add2QBuffer(readings.y, &yAccQBuff);
    add2QBuffer(readings.z, &zAccQBuff);
}
