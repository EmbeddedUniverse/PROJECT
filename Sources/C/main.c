/*
 * main.c
 */

#include <stdio.h>

#include "../../Includes/mathematic.h"

main(void){

    float signal[5] = { 0.0 , 2.0 , 3.5 , 4.7 , 1.2 };

    float*corrArr;
    corrArr = autocorrelate(signal, sizeof(signal));

    printf("-----------------------------------");

    int i,nbData;

    nbData = ((2*sizeof(signal)-sizeof(float))/sizeof(float));

    for (i = 0 ; i < nbData ; ++i){
        printf("\n%f\n", corrArr[i]);
    }

}
