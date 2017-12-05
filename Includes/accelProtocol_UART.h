/*
 * accDecode.h
 *
 *  Created on: 8 nov. 2017
 *      Author: dene2303
 */

#ifndef INCLUDES_ACCELPROTOCOL_UART_H_
#define INCLUDES_ACCELPROTOCOL_UART_H_

#include "constants.h"

#define CONV_POINTFLOTTANT 0

#define FRAME_LENGTH 16

#define X_PREFIX 'x'
#define Y_PREFIX 'y'
#define Z_PREFIX 'z'
#define STOP_BYTE END_OF_TRANSMISSION

#define X_PREFIX_INDEX  0
#define Y_PREFIX_INDEX  5
#define Z_PREFIX_INDEX  10
#define STOP_BYTE_INDEX 15

#define X_BYTES_INDEX 1
#define Y_BYTES_INDEX 6
#define Z_BYTES_INDEX 11

typedef struct{
    float x;
    float y;
    float z;
    short errors;
}AccelDecodedData;

// Callback function type
typedef void (*DecodedCallBack) (AccelDecodedData);

AccelDecodedData convertAccString2Floats(const char* accString);

void aggregateAccelBytes(unsigned char newChar);
void setDecodeCallBack(DecodedCallBack callBack);

#endif /* INCLUDES_ACCELPROTOCOL_UART_H_ */
