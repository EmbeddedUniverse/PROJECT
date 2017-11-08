/*
 * mathematic.c
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#include "mathematic.h"

float* autocorrelate(float* ResultAutocorr, float* signal, int sizeInBytesSignal)
{
    int i,n =0;
    int N = sizeInBytesSignal;

    int resultLength = 2 * N - 1;

    //Initialize ResultAutocorr array to 0
    for(i=0; i < resultLength; i++ )
    {
        ResultAutocorr[i]=0;
    }

    //Autocorrelate
    for(i = 0; i < (resultLength); ++i)
    {
        int k = i - (N-1);
        int start = (k > 0) ? k : 0;
        int end = (k < 0) ? i : N-1;

        for(n = start; n <= end; ++n)
            ResultAutocorr[i] += signal[n] * signal[n-k];
    }
    return ResultAutocorr;
}


/* float* autocorrelate(float* signal, int nbDataSignal){

    // Array containing the autocorrelation results for each value of "k"
    float*corrArr = (float*)malloc(2*sizeof(float)*nbDataSignal-sizeof(float));

    // Variables needed for the algorythm
    short corrArrIndex, dSignalIndex, i, k, limit, delay = 0;

        for (k = -(nbDataSignal-1) ; k < nbDataSignal ; ++k){
            // Limiting the number of iteration when summing only the non-null products
            limit++;

            // Starting the delayed signal index at the first n-point of the delayed signal corresponding to the first point of the original signal
            if( k <= 0 )
                dSignalIndex = -k;
            else if( k > 0 ){
                // Since it's always increasing (for performance purpose), it needs a decrementation of 2 units, to actually decrement normally
                limit = limit - 2;
                // The first point of the delayed signal always is the first point to multiply with when k > 0
                dSignalIndex = 0;
                // Delaying now progressively the original signal instead and using the previous delayed signal as an original signal
                delay++;
            }

            for (i = 0 ; i < limit ; ++i){
                corrArr[corrArrIndex] = corrArr[corrArrIndex] + signal[i + delay] * signal[dSignalIndex];
                // Delaying progressively
                dSignalIndex++;
            }
            corrArrIndex++;
        }
    return corrArr;
}*/
