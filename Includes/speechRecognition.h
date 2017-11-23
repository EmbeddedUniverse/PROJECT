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
#define NB_INTER_VAR 3
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

EXTERN int interVar[NB_INTER_VAR];

EXTERN bool flagRecognition;

void initSpeechRecognition(void);
void restartInterVar(void);
bool speechRecognitionBands(short sample[]);
bool speechRecognitionBand(short sample[], unsigned short nthFilter);
void hanning(short sample[]);
void convertIn2Q13(float sample[], short sample2Q13[]);
void filter(short sample[], unsigned short nthFilter);
short sat_16bits(int buffer);
void fft(short sample[]);
bool recognition(float absFFTResult[]);

#endif /* INCLUDES_SPEECHRECOGNITION_H_ */
