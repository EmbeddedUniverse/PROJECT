/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <stdio.h>
#include "voiceDetector.h"

void run(){

    printf("\n----------------------------------------------\n");
    printf("\t    RUNNING PROGRAM\n");
    printf("----------------------------------------------\n");


    VOICE_init();

    while(1)
    {
        if (voiceSampleReady){
            //play(voiceSample, VOICE_BUFFER_LENGTH);
            //while(!readyToPlay);
            VOICE_reset();
        }
    }
}

