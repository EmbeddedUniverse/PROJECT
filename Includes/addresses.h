/*
 * Holds the hardware addresses and registers mapped to natural language names.
 * addresses.h
 *
 */

#ifndef INCLUDES_ADDRESSES_H_
#define INCLUDES_ADDRESSES_H_

/*
 * McBSPx addresses
 */
// Pin Control Register
#define PCR0 *(unsigned int*) 0x018C0024
#define PCR1 *(unsigned int*) 0x01900024
#define FSXM 11
#define FSRM 10
#define CLKXM 9
#define CLKRM 8
#define FSXP 3
#define FSRP 2
// Receive Control Register / transmit Control Register
#define RCR0 *(unsigned int*) 0x018C000C
#define RCR1 *(unsigned int*) 0x0190000C
#define XCR0 *(unsigned int*) 0x018C0010
#define XCR1 *(unsigned int*) 0x01900010
#define PHASE   *(unsigned int*) 31
#define FRLEN1  *(unsigned int*) 8
#define FRLEN2  *(unsigned int*) 24
#define WDLEN1  *(unsigned int*) 5
#define WDLEN2  *(unsigned int*) 21
#define COMPAND *(unsigned int*) 19
#define FIG     *(unsigned int*) 18
#define DATDLY  *(unsigned int*) 16
// Sample Rate Generator Register
#define SRGR0 *(unsigned int*) 0x018C0014
#define SRGR1 *(unsigned int*) 0x01900014
#define FSGM  *(unsigned int*) 28
#define CLKSM *(unsigned int*) 29
#define CLKGDV *(unsigned int*) 0

#endif /* INCLUDES_ADDRESSES_H_ */
