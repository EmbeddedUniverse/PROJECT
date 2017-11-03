/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "mathematic.h"

main(void){

    float signal[10] = { 2 , 3 , 4 , 5 , 6 , 5 , 4 , 3 , 2 , 1 };
    float output[19]={0};
    float*corrArr;
    //corrArr = autocorrelate(signal, sizeof(signal)/sizeof(float));
    autoCorrASM(&signal[0], sizeof(signal)/sizeof(float),&output[0]);
    printf("-----------------------------------");

    int i,nbData;

    nbData = ((2*sizeof(signal)-sizeof(float))/sizeof(float));

    for (i = 0 ; i < nbData ; ++i)
        printf("\n%f\n", corrArr[i]);


    FILE *f1 = fopen("../Matlab/pre_autocorrelation_data_C.txt", "w");
    if (f1 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // print in file for Matlab test
    for(i = 0 ; i < (sizeof(signal)/sizeof(float)) ; ++i)
        fprintf(f1, "%f\n", signal[i]);

    fclose(f1);

    FILE *f2 = fopen("../Matlab/autocorrelation_results_C.txt", "w");
    if (f2 == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    // print in file for Matlab test
    for (i = 0 ; i < nbData ; ++i)
        fprintf(f2, "%f\n", corrArr[i]);

    fclose(f2);
}
