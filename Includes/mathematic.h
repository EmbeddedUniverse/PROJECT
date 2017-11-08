/*
 * mathematic.h
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#ifndef INCLUDES_MATHEMATIC_H_
#define INCLUDES_MATHEMATIC_H_

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

float* autocorrelate(float* ResultAutocorr, float* signal,  int nbDataSignal);
void autocorrelateASM(float* signal,  int nbDataSignal, float* output, int nbDataResult);


#endif /* INCLUDES_MATHEMATIC_H_ */
