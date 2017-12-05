#include <string.h>
#include <stdio.h>

#define WAITING 0
#define RECORDING 1
#define ANALAZING 2

#define VOICE_DETECT_MODULE_IMPORT
#include "voiceDetector.h"
#include "C6713Helper_UdeS.h"
#include "circularBuffer.h"
#include "firFilter.h"
#include "firCoefficients64.dat"

volatile short detectionArray[DETECTION_BUFFER_LENGTH];
volatile int detectionIndex;
volatile int voiceSampleIndex;
volatile int voiceMEF;

volatile short *signalToPlay;
size_t playLength;
volatile int playIndex;

int VOICE_init()
{
    VOICE_reset();

    comm_intr(VOICE_AIC_SAMPLING_FREQ, DSK6713_AIC23_INPUT_MIC);

    return 0;
}

int VOICE_reset()
{
    memset((void *)detectionArray, 0, DETECTION_BUFFER_LENGTH * sizeof(short));
    detectionIndex = 0;
    voiceSampleReady = false;
    voiceSampleIndex = 0;
    voiceMEF = WAITING;

    return 0;
}

interrupt void c_int11(void)
{
    short newValue = input_right_sample();

    if (voiceMEF == WAITING)
    {
        detectionArray[detectionIndex++] = abs(newValue);

        unsigned long moyenneMobile = FIR_compute((short *)detectionArray, (short *)MOY_MOB64_COEFF, DETECTION_BUFFER_LENGTH);

        if (moyenneMobile > DETECTION_THRESHOLD)
        {
            voiceMEF = RECORDING;
        }

        if (detectionIndex >= DETECTION_BUFFER_LENGTH)
            detectionIndex = 0;
    }
    else if (voiceMEF == RECORDING)
    {
        voiceSample[voiceSampleIndex++] = newValue;

        if (voiceSampleIndex >= VOICE_BUFFER_LENGTH)
        {
            voiceMEF = ANALAZING;

            voiceSampleReady = true;
        }
    }

    if (playIndex < playLength)
    {
        short vToPlay = signalToPlay[playIndex++];

        output_sample((int)vToPlay << 16 | ((int)vToPlay&0xFFFF));
        return;
    }
    else
        readyToPlay = true;

    output_sample(0);
}

int play(volatile short signal[], size_t length)
{
    signalToPlay = signal;
    playLength = length;
    playIndex = 0;

    readyToPlay = false;

    return 0;
}
