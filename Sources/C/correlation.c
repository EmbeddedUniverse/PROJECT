/*
 * correlation.c
 *
 *  Created on: 26 oct. 2017
 *      Author: bole2402 gauv2903
 */

#include "../../Includes/correlation.h"


float* correlate(float* ResultCorr, float* signalX, float* signalRef, int nbDataSignalX, int nbDataSignalRef){

    int i,j;
    //Correlate
    for(i = 0; i < (nbDataSignalX+nbDataSignalRef-1); ++i)
    {
        ResultCorr[i] = 0;

        int start = 0;
        if (i >= nbDataSignalRef)
            start = i - (nbDataSignalRef-1);
        int end = i;
        if (i >= nbDataSignalX)
            end = (nbDataSignalX-1);


        for(j = start; j <= end; ++j)
            ResultCorr[i] += signalX[j] *signalRef[j-(i -(nbDataSignalRef-1))];
    }

    return ResultCorr;
}



