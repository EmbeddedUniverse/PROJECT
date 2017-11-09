/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <csl.h>
#include <dsk6713.h>
#include <dsk6713_led.h>
#include <string.h>
#include <math.h>
#include "run.h"
#include "serial.h"
#include "circularBuffer.h"
#include "accelProtocol_UART.h"
#include "constants.h"
#include "correlation.h"
#include "ReferenceSignalY_test.dat"

#define ACC_ANALYSIS_SIZE 128
#define ACC_BUFF_SIZE 2*ACC_ANALYSIS_SIZE

void handleNextReading(const char* message, size_t length);
void copyToTemp(float* src, float* srcBase, float* dst, size_t size);

#pragma DATA_ALIGN(xAccArray, ACC_BUFF_SIZE * sizeof(float))
float xAccArray[ACC_BUFF_SIZE];
circularQBuffer xAccQBuff;

#pragma DATA_ALIGN(yAccArray, ACC_BUFF_SIZE * sizeof(float))
float yAccArray[ACC_BUFF_SIZE];
circularQBuffer yAccQBuff;

#pragma DATA_ALIGN(zAccArray, ACC_BUFF_SIZE * sizeof(float))
float zAccArray[ACC_BUFF_SIZE];
circularQBuffer zAccQBuff;

// Temporary buffer before correlation
float tempSignalBuffer[ACC_ANALYSIS_SIZE];

// Output Buffer for autocorrelation
#pragma DATA_ALIGN(corrArray, ACC_BUFF_SIZE * sizeof(float))
float corrArray[ACC_ANALYSIS_SIZE*2];
circularQBuffer corrQBuff;



void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    // Setting up the circular buffer handles
    xAccQBuff = initQBuffer(xAccArray, ACC_BUFF_SIZE);
    memset(xAccArray, 0, ACC_BUFF_SIZE * sizeof(float));
    yAccQBuff = initQBuffer(yAccArray, ACC_BUFF_SIZE);
    memset(yAccArray, 0, ACC_BUFF_SIZE * sizeof(float));
    zAccQBuff = initQBuffer(zAccArray, ACC_BUFF_SIZE);
    memset(zAccArray, 0, ACC_BUFF_SIZE * sizeof(float));

    /* initialize the CSL and BSL library */
    CSL_init();
    DSK6713_init();
    DSK6713_LED_init();


    setupSerial(McBSP0, BAUD_57600, FRAME_LENGTH, handleNextReading);


    while(1){
        if (xAccQBuff.quarterFlag)
        {
            xAccQBuff.quarterFlag = false;

        }

        if (yAccQBuff.quarterFlag)
        {
            yAccQBuff.quarterFlag = false;

            copyToTemp(yAccQBuff.last, yAccArray, tempSignalBuffer, ACC_ANALYSIS_SIZE);
            correlate(corrArray, tempSignalBuffer, REFERENCESIGNALY, ACC_ANALYSIS_SIZE, NUMREFDATA);

            float max = 0;
            int i;
            for (i = 0; i < ACC_ANALYSIS_SIZE+NUMREFDATA-1; ++i){
                if (fabsf(corrArray[i]) > max)
                    max = fabsf(corrArray[i]);
            }

            if (max > SHAKE_THRESHOLD) {
                printf("Detection %f\n", max);
                DSK6713_LED_on(0);
                DSK6713_LED_on(1);
                DSK6713_LED_on(2);
                DSK6713_LED_on(3);
            }
            else {
                //printf("NonDetection %f\n", max);
                DSK6713_LED_off(0);
                DSK6713_LED_off(1);
                DSK6713_LED_off(2);
                DSK6713_LED_off(3);
            }
        }

        if (zAccQBuff.quarterFlag)
        {
            zAccQBuff.quarterFlag = false;

        }
    }
}

void copyToTemp(float* src, float* srcBase, float* dst, size_t size){

    int i;
    for(i = size -1 ; i >= 0; --i)
    {
        if (src < srcBase)
            src = src + size*2;

        dst[i] = *src--;
    }
}

void handleNextReading(const char* message, size_t length){

    AccelDecodedData readings = convertAccString2Floats(message);

    add2QBuffer(readings.x, &xAccQBuff);
    add2QBuffer(readings.y, &yAccQBuff);
    add2QBuffer(readings.z, &zAccQBuff);
}
