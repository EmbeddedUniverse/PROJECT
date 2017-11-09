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
#define LengthRefSignal 10
#define LengthResult 19

float ResultCorr[LengthResult];                 //Result of the correlation
float ResultAutocorr[LengthResult];             //Result of the autocorrelation

// Allocating memory for intermediate values
float x_norm[LengthResult];                     //Copy of the aquired sign  ///////will not be necessary for real function use, only test
float ref_norm[LengthResult];                   //Copy of the referenced signal
float tempRes[LengthResult*LengthTrame];        //temporary table for result of correlation
////////////////////////////////////////////



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

    autocorrelate(ResultAutocorr, TESTSIGNALREF, LengthTrame);

    int i =0;
    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<LengthTrame; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultAutocorr[i]/TESTSIGNALRESULT[i] < 1.00001 && ResultAutocorr[i]/TESTSIGNALRESULT[i] > 0.99999)
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
    float ReversedReference[LengthTrame];
    for(i = 0; i<LengthTrame; i++)
    {
        ReversedReference[i] = TESTSIGNALREF[i];
    }
    //////////////////////////////////////////////////////////////////

    correlate(ResultCorr, TESTSIGNALREF, ReversedReference, x_norm, ref_norm, tempRes, LengthTrame, LengthRefSignal);

    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<LengthResult; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultCorr[i]/TESTSIGNALRESULT[i] < 1.00001 && ResultCorr[i]/TESTSIGNALRESULT[i] > 0.99999)
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

    autocorrelateASM(TESTSIGNALREF, LengthTrame, ResultAutocorr, LengthResult);

    int i =0;
    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<LengthResult; i++)
    {
        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultAutocorr[i]/TESTSIGNALRESULT[i] < 1.00001 && ResultAutocorr[i]/TESTSIGNALRESULT[i] > 0.99999)
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
