/*
 * mathematic.c
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#include "../../Includes/mathematic.h"



float* autocorrelate(float* signal, int sizeInBytesSignal){

    short nbBytesCorrArr = 2*sizeInBytesSignal-sizeof(float);

    float*delayedSignal = (float*)malloc(sizeInBytesSignal);
    int nbDataSignal = sizeInBytesSignal/sizeof(float);

    float*corrArr = (float*)malloc(nbBytesCorrArr);
  //  int nbDataCorrArr = nbBytesCorrArr/sizeof(float);

    short k;
    int index;
    int i,j,m;

    for (k = -(nbDataSignal-1) ; k < nbDataSignal ; ++k){
        index = 0;
        if( k < 0 ){
            for (i = -k ; i < nbDataSignal ; ++i){
                delayedSignal[ index ] = signal[i];
                ++index;
            }

            for (j = index ; j < nbDataSignal ; ++j){
                delayedSignal[ index ] = 0.0;
                ++index;
            }
        }
        else if( k > 0 ){
            for (j = 0 ; j < (k + 1) ; ++j){
                delayedSignal[ index ] = 0.0;
                ++index;
            }

            for (i = 0 ; i < nbDataSignal ; ++i){
                delayedSignal[ index ] = signal[i];
                ++index;
            }

        }
        else if( k == 0 ){
            for(i = 0 ; i < nbDataSignal ; ++i)
                delayedSignal[i] = signal[i];
        }

        for (m = 0 ; m < nbDataSignal ; ++m){
            corrArr[ nbDataSignal - 1 + k ] = corrArr[ nbDataSignal - 1 + k ] + signal[ m ] * delayedSignal[ m ];
        }
    }


    return corrArr;
}
