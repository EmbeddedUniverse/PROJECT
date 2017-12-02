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


/***************************************************************************
    Global variables declaration :
***************************************************************************/

EXTERN volatile bool flagUART;

/***************************************************************************
    Function prototype :
***************************************************************************/

EXTERN void SPI_init();

EXTERN void sendByteUART(unsigned char data);
EXTERN unsigned char readByteUART();


#undef MAX3111_MODULE_IMPORT
#undef EXTERN

#endif

