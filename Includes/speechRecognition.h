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

#include "DSPF_sp_cfftr4_dif.h"
#include "twiddles.h"
#include "coeffsIIR.h"
#include "IIR_2ndOrder_directII.h"

// Possible values for BUFFER_LENGTH : 16, 81, 256, 625, 1296, 2401, 4096 ...
#define BUFFER_LENGTH 256
#define THRESHOLD 0
#define nbInterVar 3
#define RECOGNIZED 1
#define UNRECOGNIZED 0

float absFFTResult[BUFFER_LENGTH];
float cplxSample[2*BUFFER_LENGTH];

int sample2Q13[BUFFER_LENGTH];

int interVar[nbInterVar] ={0,0,0};

bool flagRecognition = false;

bool speechRecognitionBands(float sample[]);
bool speechRecognitionBand(float sample[], unsigned short nthFilter);
void hanning(float sample[]);
void convertIn2Q13(float sample[]);
void filter(int sample2Q13[], unsigned short nthFilter);
short sat_16bits(int sample2Q13);
void fft(int sample2Q13[]);
bool recognition(float absFFTResult[]);

#endif /* INCLUDES_SPEECHRECOGNITION_H_ */
