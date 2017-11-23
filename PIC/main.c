/*
 * File:   main.c
 * Author: KEVIN GUIMOND - GUIK1601
 *
 * Created on August 29, 2017, 6:51 PM
 */


#include <stdio.h>
#include <stdlib.h>
#include <xc.h>            /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "usart.h" 
#include <time.h>//required to use 'srand(time(NULL))'

#define _XTAL_FREQ 8000000   // Oscillator frequency for _delay()
#pragma config XINST = OFF 

// Global variables

unsigned char rxChar = 0x00;
unsigned char transferedData = 0x00;
short nextTarget[2];
bool ErrorUART = false; 
bool rxFlag = false; // USART Data Received flag
// Declarations
void interrupt rxIsr(void);
void setUARTconfig(void);
void getRandomTarget(short Target[2]);

// Main 
void main(void) {
    setUARTconfig();
    
    while(true){
        if(rxFlag){
            rxChar = RCREG1; // Read the value in the register
            //if(rxChar==){
            
            //command
            
            }
            rxFlag = false; // Reset the flag
            PIE1bits.RC1IE = 1; // Re-enable the interrupt control
            //TXREG1 = 0xf0;
            
        }
    }
}

void interrupt rxIsr(void)
{
    if(PIR1bits.RC1IF && PIE1bits.RC1IE) {
        if(RCSTAbits.FERR || RCSTAbits.OERR) {
            ErrorUART= true;
        } else {
            rxFlag = true; // Raise the flag
            PIE1bits.RC1IE = 0; // Disable the interrupt control
        }
    }
}

void setUARTconfig(void){
 // configuration des PIN UART
    RPINR0_1 =   0b01100110; 
    RPOR26_27 =  0b00010001;
    // Initial register configuration for UART
    BAUDCON1 = 0x00;
    TXSTA1 = 0x26;  // Binary : 0010 0110
    RCSTA1 = 0x90;  // Binary : 1001 0000
    SPBRG1 = 0xcf;  // Decimal: 207
   
    // Initial interrupt configuration
    IPR1bits.RC1IP = 0; // Set the UART interrupt to low priority
    PIE1bits.RC1IE = 1; // EUSART Receive Interrupt Enable bit
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1; // Asynchronous mandatory register - USART
    INTCONbits.PEIE = 1; // Asynchronous mandatory register - USART
   
    
    // Send initial signal & console clear
    TXREG1 = 0xAA;
}

void getRandomTarget(short Target[2]){
    Target[0] = rand() % 6 + 1;
    Target[1] = rand() % 2 + 1;
}