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
#include "IIR_2ndOrder_directII.h"

//ASM prototype : filter function
int IIR_secOrder(int x, int w[], const short C[]);

void initSpeechRecognition(void){
    //Add all the stuff needed to be initialized first before starting speech recognition functions
    int i;
    for(i=0 ; i < nbInterVar; i++)
        interVar[i] = 0;
}

bool speechRecognitionBands(float sample[]){
    int nthFilter, nbRecognition = 0;
    for(nthFilter = 0; nthFilter < nbFilters; nthFilter++){
        if(speechRecognitionBand(sample, nthFilter))
            nbRecognition++;
    }
    return (nbRecognition == nbFilters) ? RECOGNIZED : UNRECOGNIZED;
}

bool speechRecognitionBand(float sample[], unsigned short nthFilter){
    //hanning(sample); TODO : Figure out if a hanning window is needed
    convertIn2Q13(sample);
    filter(sample2Q13, nthFilter);
    fft(sample2Q13);
    return recognition(absFFTResult);
}

void hanning(float sample[]){
    int i;
    for (i = 0;i<BUFFER_LENGTH;i++){
        //sample[i] = HAN_WINDOW[i]*sample[i];
    }
}

void convertIn2Q13(float sample[]){
    int i;
    for(i = 0; i < BUFFER_LENGTH; i++)
        sample2Q13[i] = (int)(sample[i]*pow((double)2,13));
}

void filter(int sample2Q13[], unsigned short nthFilter){
    int x,y;
    for(x = 0; x < nbSecondOrder; x++){
        for(y = 0; y < BUFFER_LENGTH; y++){
            //Chose between ASM and C function
            IIR_2ndOrder_directII(sample2Q13[y], interVar, filters[nthFilter]->coeffsIIR[x]);
            //IIR_secOrder(sample2Q13[y], interVar, filters[nthFilter]->coeffsIIR[x][0]);

            sample2Q13[y] = (int)(filters[nthFilter]->globalGain[x])*sample2Q13[y];   // 2Q13 x 15Q13 = 17Q26
            sample2Q13[y] = (sample2Q13[y]>>13);                                      // y->17Q13
            sat_16bits(sample2Q13[y]);                                                // y->2Q13
        }
    }
}

short sat_16bits(int x){
    short r;

    if (x>(pow(2,15)-1))
        r =(short)pow(2,15)-1;
    else if (x<-pow(2,15))
        r = (short)-pow(2,15);
    else
        r = (short)x;

    return r;
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
