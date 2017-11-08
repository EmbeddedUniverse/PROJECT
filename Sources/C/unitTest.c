/*
 * unitTest.c
 *
 *  Created on: 2 nov. 2017
 *      Author: para2709
 */

#include "test.h"
#include "unitTest.h"
#include "correlation.h"
#include "mathematic.h"
#include "TestSignal.dat"
#include "TestSignalResult.dat"

//Parameter for correlation and autocorrelation
#define LengthTrame 10
#define LengthResult 19

float ResultCorr[LengthResult];
float ResultAutocorr[LengthResult];

float x_norm[LengthResult];
float ref_norm[LengthResult];
float tempRes[LengthResult*LengthTrame];
///////////////////////////////////////////////



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
    strncpy(test.author,"Anthony Parris & Edouard Denommee", 50);
    strncpy(test.reviser,"Valerie Gauthier", 100);
    strncpy(test.description,"Test if the funciton autocorrelate() in C is funcitonal", 100);
    test.ID = 1;
    strncpy(test.result,FAIL, 5);

    autocorrelate(ResultAutocorr, SIGNALREF, TESTSIGNAL);

    int i =0;
    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<TESTSIGNAL; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultAutocorr[i]/SIGNALRES[i] < 1.00001 && ResultAutocorr[i]/SIGNALRES[i] > 0.99999)
        {
            strncpy(test.result, PASS, 5);
        }
        else
        {
            strncpy(test.result, FAIL, 5);
            break;
        }
    }
    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
}

void correlateTest(){
    nTest test;
    strncpy(test.title,"TEST'S CORRELATE TEST", 50);
    strncpy(test.author,"V.G. & E.B.", 50);
    strncpy(test.reviser,"NAN", 100);
    strncpy(test.description,"Test of the function correlate() in C ", 100);
    test.ID = 2;
    strncpy(test.result,FAIL, 5);


    //Copy of the signal WILL NOT BE NECESSARY WITH SAMPLING""""""""""
    int i = 0;
    float signalreference[TESTSIGNAL];
    for(i = 0; i<TESTSIGNAL; i++)
    {
        signalreference[i] = SIGNALREF[i];
    }

    correlate(ResultCorr, SIGNALREF, signalreference, x_norm, ref_norm, tempRes, TESTSIGNAL, TESTSIGNAL);

    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<TESTSIGNAL; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultCorr[i]/SIGNALRES[i] < 1.00001 && ResultCorr[i]/SIGNALRES[i] > 0.99999)
        {
            strncpy(test.result, PASS, 5);
        }
        else
        {
            strncpy(test.result, FAIL, 5);
            break;
        }
    }
    //Automatically prints your results
    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
}

void autocorrelateASMTest(){
    nTest test;
    strncpy(test.title,"TEST'S AUTOCORRELATION ASM", 50);
    strncpy(test.author,"Philippe Marcou", 50);
    strncpy(test.reviser,"Valerie Gauthier", 100);
    strncpy(test.description,"Test if the funciton autocorrelate() in ASM is funcitonal", 100);
    test.ID = 1;
    strncpy(test.result,FAIL, 5);

    autocorrelateASM(SIGNALREF, LengthTrame, ResultAutocorr, LengthResult);

    int i =0;
    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<TESTSIGNAL; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultAutocorr[i]/SIGNALRES[i] < 1.00001 && ResultAutocorr[i]/SIGNALRES[i] > 0.99999)
        {
            strncpy(test.result, PASS, 5);
        }
        else
        {
            strncpy(test.result, FAIL, 5);
            break;
        }
    }
    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
}
