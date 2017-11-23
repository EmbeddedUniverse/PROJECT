#include <string.h>
#include <stdio.h>

#define VOICE_DETECT_MODULE_IMPORT
#include "voiceDetector.h"
#include "C6713Helper_UdeS.h"
#include "circularBuffer.h"

volatile short detectionArray[DETECTION_BUFFER_LENGTH];
volatile int detectionIndex;
volatile int voiceSampleIndex;
volatile bool recording;

int VOICE_init()
{
    VOICE_reset();

    comm_intr(DSK6713_AIC23_FREQ_16KHZ, DSK6713_AIC23_INPUT_MIC);

    return 0;
}

int VOICE_reset()
{
    //memset((void *)detectionArray, 0, DETECTION_BUFFER_LENGTH * sizeof(short));
    detectionIndex = 0;
    voiceSampleReady = false;
    voiceSampleIndex = 0;
    recording = false;

    //IRQ_enable(11);

    return 0;
}

interrupt void c_int11(void)
{
    short newValue = input_right_sample();

//    if (!recording)
//    {
//        detectionArray[detectionIndex++] = abs(newValue);
//
//        unsigned int i, sum = 0;
//
//        for (i = 0; i < DETECTION_BUFFER_LENGTH; ++i)
//            sum += detectionArray[i];
//
//        if (sum > DETECTION_THRESHOLD)
//        {
//            recording = true;
//            printf("Started rec; val : %d\r\n", sum);
//        }
//
//        if (detectionIndex >= DETECTION_BUFFER_LENGTH)
//            detectionIndex = 0;
//    }
//    else
//    {
//        voiceSample[voiceSampleIndex++] = newValue;
//
//        if (voiceSampleIndex >= VOICE_BUFFER_LENGTH)
//        {
//            recording = false;
//
//            voiceSampleReady = true;
//
//            // Stop interrupting for new samples until the record is analysed
//            IRQ_disable(11);
//        }
//    }
//
//    output_sample(0);
}
