/*
 * correlation.c
 *
 *  Created on: 26 oct. 2017
 *      Author: bole2402 gauv2903
 */

#include "../../Includes/correlation.h"

float* correlate(float* signalX, float* signalRef, int nbDataSignalX, int nbDataSignalRef){

    float *x_norm;
    float *ref_norm;
    float*ResultCorr = (float*)malloc(sizeof(float)*(nbDataSignalX+nbDataSignalRef)-sizeof(float));
    int lengthSignal;
    int i,j;

    if(nbDataSignalX < nbDataSignalRef)
    {
       x_norm = (float*)malloc(nbDataSignalRef*sizeof(float));
       for(j=0; j<nbDataSignalRef; j++){
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
       ref_norm = (float*)malloc(nbDataSignalX*sizeof(float));
       for(j=0; j<nbDataSignalX; j++){
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





    ResultCorr = 0;

return ResultCorr;
}
