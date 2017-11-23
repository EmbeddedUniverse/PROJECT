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
#define nbInterVar 3
#define RECOGNIZED 1
#define UNRECOGNIZED 0

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

EXTERN int interVar[nbInterVar];

EXTERN bool flagRecognition;

void initSpeechRecognition(void);
bool speechRecognitionBands(float sample[]);
bool speechRecognitionBand(float sample[], unsigned short nthFilter);
void hanning(float sample[]);
void convertIn2Q13(float sample[]);
void filter(int sample2Q13[], unsigned short nthFilter);
short sat_16bits(int sample2Q13);
void fft(int sample2Q13[]);
bool recognition(float absFFTResult[]);

#endif /* INCLUDES_SPEECHRECOGNITION_H_ */
