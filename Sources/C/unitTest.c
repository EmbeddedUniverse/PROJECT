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
#include "speechRecognition.h"
#include "TestSignal.dat"
#include "TestSignal2.dat"
#include "TestSignalResult.dat"
#include "TestSignalResult2.dat"
#include "TestVoiceSamples.dat"

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

    int i;
    //////////////////////////////////////////////////////////////////

    correlate(ResultCorr, SIGNALREF2, TESTSIGNALREF, TESTSIGNAL2, TESTSIGNAL);

    //If the results of each sample of the algorithm match the results obtained with xcorr the test is passed
    for(i=0; i<TESTSIGNALRES2; i++)
    {
        printf("Excpected: %.5f \t Result: %.5f \n", SIGNALRES2[i], ResultCorr[i]);

        // The result of the algorithm is within a 0.001% margin of the result obtained with xcorr
        if(ResultCorr[i]/SIGNALRES2[i] < 1.00001 && ResultCorr[i]/SIGNALRES2[i] > 0.99999)
        {
            strncpy(test.result, PASS, 5);
        }
        else
        {
            strncpy(test.result, FAIL, 5);
            //break;
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
    test.ID = 3;
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

void speechRecognitionTest(){
    nTest test;
    strncpy(test.title,"SPEECH RECOGNITION TEST", 50);
    strncpy(test.author,"Anthony Parris", 50);
    strncpy(test.reviser,"Edouard Denommee", 100);
    strncpy(test.description,"Test the recognition of the sound 'PEW'", 100);
    test.ID = 4;
    strncpy(test.result,FAIL, 5);

//    while(timer != 5){
//        if(speechRecognition(sample[]))
//            strncpy(test.result, PASS, 5);
//    }


    // Verifying good samples are recognized
    int i;
    for (i = 0; i < NB_TEST_PEW_GOOD; ++i)
    {
        bool detected = speechRecognition(TEST_PEW_GOOD[i]);
        if (! detected ){
            printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
            return;
        }
    }

    // Verifying bad samples are not recognized
    for (i = 0; i < NB_TEST_PEW_BAD; ++i)
    {
        bool detected = speechRecognition(TEST_PEW_BAD[i]);
        if (detected){
            printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
            return;
        }
    }

    strncpy(test.result,PASS, 5);
    printTestResult(test.title, test.author, test.reviser, test.description, test.ID, test.result);
}
