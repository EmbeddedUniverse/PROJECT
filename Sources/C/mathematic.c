/*
 * mathematic.c
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#include "../../Includes/mathematic.h"



float* autocorrelate(float* signal, int sizeInBytesSignal){

    int nbDataSignal = sizeInBytesSignal/sizeof(float);

    short nbBytesCorrArr = 2*sizeInBytesSignal-sizeof(float);
    float*corrArr = (float*)malloc(nbBytesCorrArr);

    short corrArrIndex, dSignalIndex, i, k, limit, delay;
    limit = 0;
    delay = 0;
    for (k = -(nbDataSignal-1) ; k < nbDataSignal ; ++k){
        if( k < 0 ){
            limit++;
            dSignalIndex = -k;
        }
        else if( k > 0 ){
            limit--;
            dSignalIndex = 0;
            delay++;
        }
        else if( k == 0 ){
            limit++;
            dSignalIndex = 0;
        }
        for (i = 0 ; i < limit ; ++i){
            corrArr[corrArrIndex] = corrArr[corrArrIndex] + signal[i + delay] * signal[dSignalIndex];
            dSignalIndex++;
        }
        corrArrIndex++;
    }
    return corrArr;
}
