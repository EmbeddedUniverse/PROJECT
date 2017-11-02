/*
 * main.c
 */

#include <stdio.h>

#include "../../Includes/mathematic.h"

main(void){

    float signal[3] = { 4.3 ,2.5 , 1.7 };
    float output[5] = {0};
    int sizeOfSignal = sizeof(signal)/sizeof(float);
    int sizeOfOutput = sizeof(output)/sizeof(float);

    float*corrArr;
    //corrArr = autocorrelate(signal, sizeof(signal)/sizeof(float));
    autoCorrASM(&signal[0], sizeOfSignal, &output[0],sizeOfOutput);
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
