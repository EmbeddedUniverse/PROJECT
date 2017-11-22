/*
 * speechRecognition.h
 *
 *  Created on: 19 nov. 2017
 *      Author: para2709
 */

#ifndef INCLUDES_SPEECHRECOGNITION_H_
#define INCLUDES_SPEECHRECOGNITION_H_

#include <stdbool.h>
#include <math.h>

// Possible values for BUFFER_LENGTH : 16, 81, 256, 625, 1296, 2401, 4096 ...
#define BUFFER_LENGTH 256
#define THRESHOLD 0

/***************************************************************************
    Set EXTERN macro :
***************************************************************************/

#ifdef SPEECHRECOGNITION_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN float absFFTResult[BUFFER_LENGTH];
EXTERN float cplxSample[2*BUFFER_LENGTH];

EXTERN int sample2Q13[BUFFER_LENGTH];

EXTERN bool flagRecognition;

bool speechRecognition(float sample[]);
void convertIn2Q13(float sample[]);
void filter(int sample2Q13[]);
void fft(int sample2Q13[]);
bool recognition(float absFFTResult[]);

#endif /* INCLUDES_SPEECHRECOGNITION_H_ */
