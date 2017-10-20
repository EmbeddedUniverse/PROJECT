/*
 * mathematic.c
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#include "../../Includes/mathematic.h"



float* autocorrelate(float* signal, int sizeInBytesSignal){

    int N = sizeInBytesSignal / sizeof(float);

    int resultLength = 2 * N - 1;

    float * result = (float *) malloc(resultLength * sizeof(float));

    for (int i = 0; i < resultLength-1; ++i)
    {
        int k = i - (N-1);
        int start = (k > 0) ? k : 0;
        int end = (k < 0) ? i : N-1;
        int sum = 0;

        for(int n = start; n <= end; ++n)
            result[i] += signal[n] * signal[n-k];
    }

    return result;
}
