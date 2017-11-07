/*
 * serial.h
 *
 *  Created on: 2 nov. 2017
 *      Author: dene2303
 *
 *  Serial  communication module
 */

#ifndef INCLUDES_SERIAL_H_
#define INCLUDES_SERIAL_H_

typedef enum{
    BAUD_110 = 110,
    BAUD_300 = 300,
    BAUD_1200 = 1200,
    BAUD_2400 = 2400,
    BAUD_4800 = 4800,
    BAUD_9600 = 9600,
    BAUD_19200 = 19200,
    BAUD_38400 = 38400,
    BAUD_57600 = 57600,
    BAUD_115200 = 115200,
    BAUD_230400 = 320400
}BaudRate;

typedef enum {
    McBSP0 = 0,
    McBSP1 = 1
}McBSP_Port;

// Serial functions
int setupSerial (McBSP_Port port, BaudRate baud);
int setReceiveFunction(void (*callback)(char));

#endif /* INCLUDES_SERIAL_H_ */
