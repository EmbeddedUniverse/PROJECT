/*
 * speechRecognition.c
 *
 *  Created on: 19 nov. 2017
 *      Author: para2709
 */

#include "speechRecognition.h"

bool speechRecognition(float sample[]){
    //hamming(sample); TODO : Figure out if a hamming window is needed
    filter(sample);
    fft(sample);
    return recognition(absFFTResult);
}

void filter(float sample[]){

}

void fft(float sample[]){
    int i;
    for (i = 0 ; i < BUFFER_LENGTH ; i++){
        if(i%2 == 0)
            cplxSample[i] = cplxSample[i/2];
        else
            cplxSample[i] = 0;
    }

    DSPF_sp_cfftr4_dif(cplxSample, w, BUFFER_LENGTH);

    for (i = 0 ; i < BUFFER_LENGTH ; i++)
        absFFTResult[i] = sqrt(cplxSample[2*i]*cplxSample[2*i] + cplxSample[2*i+1]*cplxSample[2*i+1]);
}

bool recognition(float absFFTResult[]){
    flagRecognition = false;

    //TODO: Figure out what is the threshold value to valid a detection ? Must it be a positive value only? Is sufficient?
    int i = 0;
    while(i < BUFFER_LENGTH && !flagRecognition){
        if(absFFTResult[i] > THRESHOLD)
            flagRecognition = true;
        i++;
    }

    return flagRecognition;
}
