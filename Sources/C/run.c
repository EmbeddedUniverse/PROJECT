#include <stdio.h>
#include <dsk6713.h>
#include <dsk6713_led.h>
#include <string.h>
#include "voiceDetector.h"
#include "piouRecognition.h"
#include "communication.h"
#include "accelProtocol_UART.h"
#include "circularBuffer.h"
#include "ReferenceSignalY_test.dat"
#include "correlation.h"

#define ACC_ANALYSIS_SIZE 128
#define ACC_BUFF_SIZE 2*ACC_ANALYSIS_SIZE


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

void handleNextReading(AccelDecodedData reading);
void copyToTemp(float* src, float* srcBase, float* dst, size_t size);

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

    DSK6713_init();
    DSK6713_LED_init();

    VOICE_init();
    PIOU_init();

    setDecodeCallBack(handleNextReading);
    COM_setReceiveCallBack(aggregateAccelBytes, ACCEL);
    COM_init();


    while(1)
    {
        if (xAccQBuff.quarterFlag)
        {
            xAccQBuff.quarterFlag = false;

            yAccQBuff.quarterFlag = false;

            copyToTemp(yAccQBuff.last, yAccArray, tempSignalBuffer, ACC_ANALYSIS_SIZE);
            correlate(corrArray, tempSignalBuffer, REFERENCESIGNALY, ACC_ANALYSIS_SIZE, NUMREFDATA);

            float max = 0;
            int i;
            for (i = 0; i < ACC_ANALYSIS_SIZE+NUMREFDATA-1; ++i){
                if (fabsf(corrArray[i]) > max)
                    max = fabsf(corrArray[i]);
            }

            copyToTemp(yAccQBuff.last, yAccArray, tempSignalBuffer, ACC_ANALYSIS_SIZE);
            correlate(corrArray, tempSignalBuffer, REFERENCESIGNALY, ACC_ANALYSIS_SIZE, NUMREFDATA);

            for (i = 0; i < ACC_ANALYSIS_SIZE+NUMREFDATA-1; ++i){
                if (fabsf(corrArray[i]) > max)
                    max = fabsf(corrArray[i]);
            }

            if (max > SHAKE_THRESHOLD) {
                //printf("Detection %f\n", max);
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

            COM_send(0xAF, PIC);
        }
    }
}

void handleNextReading(AccelDecodedData reading){

    add2QBuffer(reading.x, &xAccQBuff);
    add2QBuffer(reading.y, &yAccQBuff);
    add2QBuffer(reading.z, &zAccQBuff);
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
