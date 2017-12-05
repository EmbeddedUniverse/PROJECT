#include <stdio.h>
#include <string.h>
#include "motion.h"
#include "constants.h"
#include "correlation.h"
#include "accelProtocol_UART.h"
#include "circularBuffer.h"
#include "ReferenceSignalY_test.dat"

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


void MOTION_init()
{
    // Setting up the circular buffer handles
    xAccQBuff = initQBuffer(xAccArray, ACC_BUFF_SIZE);
    memset(xAccArray, 0, ACC_BUFF_SIZE * sizeof(float));
    yAccQBuff = initQBuffer(yAccArray, ACC_BUFF_SIZE);
    memset(yAccArray, 0, ACC_BUFF_SIZE * sizeof(float));
    zAccQBuff = initQBuffer(zAccArray, ACC_BUFF_SIZE);
    memset(zAccArray, 0, ACC_BUFF_SIZE * sizeof(float));

    setDecodeCallBack(handleNextReading);
}

bool MOTION_readyToAnalyze()
{
    return xAccQBuff.quarterFlag && yAccQBuff.quarterFlag && zAccQBuff.quarterFlag;
}

bool MOTION_detectReload()
{
    copyToTemp(yAccQBuff.last, yAccArray, tempSignalBuffer, ACC_ANALYSIS_SIZE);
    correlate(corrArray, tempSignalBuffer, REFERENCESIGNALY, ACC_ANALYSIS_SIZE, NUMREFDATA);

    float max = 0;
    int i;
    for (i = 0; i < ACC_ANALYSIS_SIZE+NUMREFDATA-1; ++i){
        if (fabsf(corrArray[i]) > max)
            max = fabsf(corrArray[i]);
    }

    yAccQBuff.quarterFlag = false;

    return (max > SHAKE_THRESHOLD);
}

bool MOTION_detectModeChange()
{
    return true;
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
