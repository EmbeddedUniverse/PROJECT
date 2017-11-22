/*
 * speechRecognition.c
 *
 *  Created on: 19 nov. 2017
 *      Author: para2709
 */

#define SPEECHRECOGNITION_MODULE_IMPORT

#include "speechRecognition.h"
#include "DSPF_sp_cfftr4_dif.h"
#include "twiddles.h"
#include "coeffsIIR.h"

int IIR_secOrder(int sample2Q13, int w[], const short C[]);

bool speechRecognition(float sample[]){
    //hamming(sample); TODO : Figure out if a hamming window is needed
    convertIn2Q13(sample);
    filter(sample2Q13);
    fft(sample2Q13);
    return recognition(absFFTResult);
}

void convertIn2Q13(float sample[]){
    int i;
    for(i = 0; i < BUFFER_LENGTH; i++)
        sample2Q13[i] = (int)(sample[i]*pow((double)2,13));
}

void filter(int sample2Q13[]){
    int i,y;
    for(i = 0; i < nbFilters; i++){
        for(y = 0; y < nbSecondOrder; y++){
            //IIR_secOrder(sample2Q13, , );
        }
    }
}

void fft(int sample2Q13[]){
    int i;
    for(i = 0 ; i < BUFFER_LENGTH ; i++){
        if(i%2 == 0)
            cplxSample[i] = sample2Q13[i/2];
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
