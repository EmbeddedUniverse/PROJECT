/*
 * main.c
 */

#include <stdio.h>

#include "../../Includes/mathematic.h"
#include "../../Includes/correlation.h"

int ResultCorr=0;

main(void){


    float signal[12] = { 0.06234 , 2.032 , 3.578 , 4.7654 , 1.8233 , 9.8768 , 8.624 , 5.7653 , 7.76584 , 4.3 , 6.77777 , 0.999 };

    float*corrArr;
    corrArr = autocorrelate(signal, sizeof(signal)/sizeof(float));

    correlate(signal, signal, sizeof(signal)/sizeof(float), sizeof(signal)/sizeof(float) );
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
