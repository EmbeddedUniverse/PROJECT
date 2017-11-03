/*
 * mathematic.h
 *
 *  Created on: 18 oct. 2017
 *      Author: para2709
 */

#ifndef INCLUDES_MATHEMATIC_H_
#define INCLUDES_MATHEMATIC_H_

float* autocorrelate(float* signal,  int nbDataSignal);

void autocorrelateASM(float* signal,  int nbDataSignal, float* output);


#endif /* INCLUDES_MATHEMATIC_H_ */
