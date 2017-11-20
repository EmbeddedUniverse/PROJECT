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

// Possible values for BUFFER_LENGTH : 16, 81, 256, 625, 1296, 2401, 4096 ...
#define BUFFER_LENGTH 256
#define THRESHOLD 0

float absFFTResult[BUFFER_LENGTH];
float cplxSample[2*BUFFER_LENGTH];

bool flagRecognition = false;

bool speechRecognition(float sample[]);
void filter(float sample[]);
void fft(float sample[]);
bool recognition(float absFFTResult[]);

#endif /* INCLUDES_SPEECHRECOGNITION_H_ */
