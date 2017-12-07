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

void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");

    DSK6713_init();
    DSK6713_LED_init();
    int i;
    for (i = 0; i< 4; ++i)
        DSK6713_LED_off(i);

    VOICE_init();
    PIOU_init();

    while(1)
    {
        if (voiceSampleReady)
        {
            play(voiceSample, VOICE_BUFFER_LENGTH);
            while(!readyToPlay);

            if (detectPiou(voiceSample))
            {
                DSK6713_LED_on(0);
                DSK6713_LED_on(1);
                DSK6713_LED_on(2);
                DSK6713_LED_on(3);
                DSK6713_waitusec(125000);
                DSK6713_LED_off(0);
                DSK6713_LED_off(1);
                DSK6713_LED_off(2);
                DSK6713_LED_off(3);
            }

            VOICE_reset();
        }
    }
}

