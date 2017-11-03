/*
 * unitTest.c
 *
 *  Created on: 2 nov. 2017
 *      Author: para2709
 */

#include "test.h"
#include "unitTest.h"

void templateTest(){
    nTest test;
    strncpy(test.title,"TEST'S TEMPLATE TEST", 50);
    strncpy(test.author,"Anthony Parris", 50);
    strncpy(test.reviser,"Edouard Denommee", 100);
    strncpy(test.description,"Test if the actual implemented template for\n\t all the tests is functional and useful", 100);
    test.ID = 0;
    strncpy(test.result,FAIL, 5);

    //Normally, add your code lines and test what you need, change the struct's members according ot your specific test
    //Your code...

    //In this example, since it's about testing the template for the tests, then the test is completed/passed
    strncpy(test.result, PASS, 5);

    //Automatically prints your results
    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);

}

void autocorrelateTest(){
    nTest test;
    strncpy(test.title,"TEST'S AUTOCORRELATION", 50);
    strncpy(test.author,"Anthony Parris", 50);
    strncpy(test.reviser,"None", 100);
    strncpy(test.description,"Test if the funciton autocorrelate() in C is funcitonal", 100);
    test.ID = 1;
    strncpy(test.result,FAIL, 5);

    //    float signal[10] = { 2 , 3 , 4 , 5 , 6 , 5 , 4 , 3 , 2 , 1 };
    //    float output[19]={0};
    //    float*corrArr;
    //    //corrArr = autocorrelate(signal, sizeof(signal)/sizeof(float));
    //    autoCorrASM(&signal[0], sizeof(signal)/sizeof(float),&output[0]);
    //    printf("-----------------------------------");
    //
    //    int i,nbData;
    //
    //    nbData = ((2*sizeof(signal)-sizeof(float))/sizeof(float));
    //
    //    for (i = 0 ; i < nbData ; ++i)
    //        printf("\n%f\n", corrArr[i]);
    //
    //
    //    FILE *f1 = fopen("../Matlab/pre_autocorrelation_data_C.txt", "w");
    //    if (f1 == NULL)
    //    {
    //        printf("Error opening file!\n");
    //        exit(1);
    //    }
    //
    //    // print in file for Matlab test
    //    for(i = 0 ; i < (sizeof(signal)/sizeof(float)) ; ++i)
    //        fprintf(f1, "%f\n", signal[i]);
    //
    //    fclose(f1);
    //
    //    FILE *f2 = fopen("../Matlab/autocorrelation_results_C.txt", "w");
    //    if (f2 == NULL)
    //    {
    //        printf("Error opening file!\n");
    //        exit(1);
    //    }
    //
    //    // print in file for Matlab test
    //    for (i = 0 ; i < nbData ; ++i)
    //        fprintf(f2, "%f\n", corrArr[i]);
    //
    //    fclose(f2);

    strncpy(test.result, FAIL, 5);

    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
}
