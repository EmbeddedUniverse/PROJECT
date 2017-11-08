/*
 * Holds the tests.
 * correlation.h
 *
 */

#ifndef CORRELATION_H_
#define CORRELATION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

float* correlate(float* ResultCorr, float* signalX, float* signalRef, float* x_norm, float* ref_norm, float* tempRes, int nbDataSignalX, int nbDataSignalRef);


#endif
