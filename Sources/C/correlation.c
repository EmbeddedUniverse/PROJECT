/*
 * correlation.c
 *
 *  Created on: 26 oct. 2017
 *      Author: bole2402 gauv2903
 */

#include "../../Includes/correlation.h"


float* correlate(float* ResultCorr, float* signalX, float* signalRef, float* x_norm, float* ref_norm, float* tempRes, int nbDataSignalX, int nbDataSignalRef){

    int lengthSignal;
    int i,j;

    if(nbDataSignalX < nbDataSignalRef)
    {
       for(j=0; j<(nbDataSignalRef-nbDataSignalX); j++){
           for(i = 0; i<nbDataSignalX; i++){
               x_norm[i] = signalX[i];
           }
           x_norm[nbDataSignalX+j] = 0;
       }
       ref_norm = signalRef;
       lengthSignal = nbDataSignalRef;
    }

    if(nbDataSignalX > nbDataSignalRef)
    {
       for(j=0; j<(nbDataSignalX-nbDataSignalRef); j++){
           for(i = 0; i<nbDataSignalRef; i++){
               ref_norm[i] = signalRef[i];
           }
           ref_norm[nbDataSignalRef+j] = 0;
       }
       x_norm = signalX;
       lengthSignal = nbDataSignalX;
    }

    if(nbDataSignalX == nbDataSignalRef)
    {
       x_norm = signalX;
       ref_norm = signalRef;
       lengthSignal = nbDataSignalX;
    }

    //Initialize ResultCorr array to 0
    for(i=0; i < (lengthSignal*2-1); i++ )
    {
        ResultCorr[i] = 0;
    }

    //Correlate
    for(i = 0; i < (lengthSignal*2-1); ++i)
    {
        int k = i - (lengthSignal-1);
        int start = (k > 0) ? k : 0;
        int end = (k < 0) ? i : lengthSignal-1;

        for(j = start; j <= end; ++j)
            ResultCorr[i] += x_norm[j] * ref_norm[j-k];
    }

    return ResultCorr;
}
