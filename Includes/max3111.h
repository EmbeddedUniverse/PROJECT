#pragma once
#ifndef _SPI_MODULE_
#define _SPI_MODULE_

/***************************************************************************
    Include headers :
***************************************************************************/


// standard libraries
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


/***************************************************************************
    Set EXTERN macro :
***************************************************************************/

#ifdef MAX3111_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

/***************************************************************************
    Constants declaration :
***************************************************************************/


/***************************************************************************
    Types declaration here :
***************************************************************************/

typedef enum{
    BAUD_600 = 15,
    BAUD_1200 = 14,
    BAUD_2400 = 13,
    BAUD_4800 = 12,
    BAUD_9600 = 11,
    BAUD_19200 = 10,
    BAUD_38400 = 9,
    BAUD_76800 = 8,
    BAUD_1800 = 7,
    BAUD_3600 = 6,
    BAUD_7200 = 5,
    BAUD_14400 = 4,
    BAUD_28800 = 3,
    BAUD_57600 = 2,
    BAUD_115200 = 1,
    BAUD_230400 = 0
}BaudRate;

/***************************************************************************
    Global variables declaration :
***************************************************************************/

/***************************************************************************
    Function prototype :
***************************************************************************/

EXTERN void SPI_init();
EXTERN int MAX3111_init(BaudRate baud);
EXTERN bool sendByteUART(unsigned char data);
EXTERN unsigned char readByteUART();
unsigned short flushFIFO();
bool readNewByteUART(unsigned char * receivedValue);

#undef MAX3111_MODULE_IMPORT
#undef EXTERN

#endif

