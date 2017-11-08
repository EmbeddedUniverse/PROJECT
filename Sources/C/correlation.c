/*
 * correlation.c
 *
 *  Created on: 26 oct. 2017
 *      Author: bole2402 gauv2903
 */

#include "../../Includes/correlation.h"


float* correlate(float* ResultCorr, float* signalX, float* signalRef, float* x_norm, float* ref_norm, float* tempRes, int nbDataSignalX, int nbDataSignalRef){

    int lengthSignal;
    int i,j,k;

    if(nbDataSignalX < nbDataSignalRef)
    {
       x_norm = (float*)malloc(nbDataSignalRef*sizeof(float));
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
       ref_norm = (float*)malloc(nbDataSignalX*sizeof(float));
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

    //Instead of creating a 2d array, the temporary array will contain lengthSignal*(lengthSignal*2-1) elements.
    tempRes = (float*)malloc(sizeof(float)*(lengthSignal*(lengthSignal*2-1)));

    //Reverse ref signal normalized
    float temp;
    for(i = 0; i  < (int)floor(lengthSignal/2); i++)
    {
        temp = ref_norm[i];
        ref_norm[i] = ref_norm[(lengthSignal-1)-i];
        ref_norm[(lengthSignal-1)-i] = temp;
    }

    //fill temporary table with values of each iteration
    for (i = 0; i<lengthSignal; i++)
    {
        j = 0;
        if(i!=0)
        {
            for(k = 0; k <= (i-1); k++)
            {
                tempRes[(i*(2*lengthSignal-1)+j)] = 0;
                j++;
            }
        }
        for(k = 0; k < lengthSignal ; k++)
        {
            tempRes[(i*(2*lengthSignal-1)+j)] = x_norm[i]*ref_norm[k];
            j++;
        }
        for(k = 0; k < (lengthSignal-1-i); k++)
        {
            tempRes[(i*(2*lengthSignal-1)+j)] = 0;
            j++;
        }
    }

    //Sum each iteration of the temporary table
    for(i=0; i< (2*lengthSignal-1); i++)
    {
        for(j = 0; j < lengthSignal; j++)
        {
            ResultCorr[i] = ResultCorr[i]+ tempRes[j*(2*lengthSignal-1)+i];
        }
    }

    return ResultCorr;
}
