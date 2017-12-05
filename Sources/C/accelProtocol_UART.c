/*
 * decodeAcc.c
 *
 *  Created on: 8 nov. 2017
 *      Author: dene2303
 */

#include <accelProtocol_UART.h>

#define WAITING_FOR_X 0

volatile char accUartBuffer[FRAME_LENGTH];
int accUartBufferIndex = WAITING_FOR_X;

DecodedCallBack passedCallback;

float bytes2Float_SE(const char*);

AccelDecodedData convertAccString2Floats(const char* accString){

    AccelDecodedData result = {0, 0, 0, 0};

    if (accString[X_PREFIX_INDEX] != X_PREFIX)
        ++result.errors;

    result.x = bytes2Float_SE(&accString[X_BYTES_INDEX]);

    if (accString[Y_PREFIX_INDEX] != Y_PREFIX)
        ++result.errors;

    result.y = bytes2Float_SE(&accString[Y_BYTES_INDEX]);

    if (accString[Z_PREFIX_INDEX] != Z_PREFIX)
        ++result.errors;

    result.z = bytes2Float_SE(&accString[Z_BYTES_INDEX]);

    if (accString[STOP_BYTE_INDEX] != STOP_BYTE)
        ++result.errors;

    return result;
}

#if (CONV_POINTFLOTTANT)
float bytes2Float_SE(const char* byteArray)
{
    unsigned int number = (unsigned int)byteArray[0]
        | ((unsigned int)byteArray[1]) << 8
        | ((unsigned int)byteArray[2]) << 16
        | ((unsigned int)byteArray[3]) << 24;

    float floating = *(float*) &number;

    return floating;
}
#else
float bytes2Float_SE(const char* byteArray)
{
    int number;
    char *addr = (char*)&number;
    int i;
    for (i = 0; i < sizeof(float); ++i)
        addr[i] = byteArray[i];

    return (float)number / (float)(1<<29);
}
#endif


void aggregateAccelBytes(unsigned char newChar)
{
    if (accUartBufferIndex == WAITING_FOR_X && newChar != 'x')
        return;

    accUartBuffer[accUartBufferIndex++] = newChar;

    if (accUartBufferIndex >= FRAME_LENGTH)
    {
        accUartBufferIndex = WAITING_FOR_X;
        passedCallback(convertAccString2Floats((char *)accUartBuffer));
    }
}

void setDecodeCallBack(DecodedCallBack callBack)
{
    passedCallback = callBack;
}
