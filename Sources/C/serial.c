/*
 * serial.c
 *
 *  Created on: 2 nov. 2017
 *      Author: dene2303
 *
 *  Serial  communication functions
 */

#include "serial.h"
#include "addresses.h"
#include "constants.h"
#include "math.h"

int setupSerial (McBSP_Port port, BaudRate baud) {

    unsigned int  *PCR, *RCR, *XCR, *SRGR;

    if (port == McBSP0) {
        PCR = &PCR0;
        RCR = &RCR0;
        XCR = &XCR0;
        SRGR = &SRGR0;
    }
    else if (port == McBSP1) {
        PCR = &PCR1;
        RCR = &RCR1;
        XCR = &XCR1;
        SRGR = &SRGR1;
    }
    else
        return -1;

    // Pin Control Register
    *PCR = 0;
    *PCR |= 1 << FSXM;
    *PCR |= 1 << FSXP;
    *PCR |= 1 << CLKRM;
    *PCR |= 1 << CLKXM;

    // Receive Control Register / transmit Control Register
    *RCR = *XCR = 0;
    *RCR |= 1 << PHASE;
    *RCR |= 8 << FRLEN1;
    *RCR |= 1 << FRLEN2;
    *RCR |= 2 << WDLEN1;
    *RCR |= 1 << FIG;
    *XCR = *RCR;
    *RCR |= 1 << DATDLY;

    // Sample Rate Generator Register
    *SRGR = 0;
    *SRGR |= 1 << CLKSM;
    unsigned int clockDividerValue = roundf(CLK_SRC / (float)baud) - 1;
    *SRGR |= clockDividerValue << CLKGDV;

    return 0;
}
