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

#define _XTAL_FREQ  8000000   // Oscillator frequency for _delay()
#define reloadCode  0xAA
#define modeCode    0xBB
#define pewCode     0xCC

#pragma config XINST = OFF 

typedef enum state{
    IDLE,
    SELECT_NEW_TARGET,
    ACCUMULATE_POINTS,
    NEED_RELOAD,
    WAIT_KILL,
    END_GAME,        
}statePIC;
statePIC myState=IDLE;

typedef enum gunMode{
    MODE1,
    MODE2
}gunModeState;
gunModeState myModeState= MODE1;
// Global variables


unsigned char rxChar = 0x00;
unsigned char transferedData = 0x00;
short nextTarget[2];
int ammoLeft = 12;

bool ErrorUART      = false; 
bool rxFlag         = false; // USART Data Received flag
bool reloadFlag     = false;
bool modeFlag       = false; 
bool pewFlag        = false;
bool capteurFlag    = false;
// Declarations
void interrupt rxIsr(void);
void setUARTconfig(void);
void setPinConfig(void);
void getRandomTarget(short Target[2]);
void activateTarget(short targetNbr);
void activateLEDTarget(short targetLED[2]);
void fireShot();
void stopShot();

// Main 
void main(void) {
    setUARTconfig();
    setPinConfig();
    
    while(true){
        if(rxFlag){
            rxChar = RCREG1; // Read the value in the register       
            rxFlag = false; // Reset the flag
            PIE1bits.RC1IE = 1; // Re-enable the interrupt control
            TXREG1 = 0xf0;
            
        }
    }
    
    while(1){
    switch(myState){
        case IDLE :
            
            
            break;
            
        case SELECT_NEW_TARGET:
            getRandomTarget(nextTarget);
            activateTarget(nextTarget[0]);
            activateLEDTarget(nextTarget);
            myState= WAIT_KILL;
            break;
            
        case ACCUMULATE_POINTS:
            
            break;
        
        case WAIT_KILL:
            if(reloadFlag){
                ammoLeft= 12;              
                reloadFlag=false;
                PIE1bits.RC1IE = 1;
            }
            
            else if (modeFlag){
                if (myModeState == MODE1){
                    myModeState=MODE2;                  
                } 
                else{
                    myModeState=MODE1;
                }

                modeFlag=false;
                PIE1bits.RC1IE = 1;
            }
            
            else if (pewFlag && ammoLeft !=0){
                fireShot();
                ammoLeft-=1;
                pewFlag=false;
                PIE1bits.RC1IE = 1;
            }
            else if(ammoLeft==0){
                myState= NEED_RELOAD;
            }
            else if(capteurFlag){
                capteurFlag=false;
                PIE1bits.RC1IE = 1;
                myState= ACCUMULATE_POINTS;
            }
            
            stopShot();
            break;
        
        case END_GAME:
            
            break;
                   
    }        
    }
    
}

void interrupt rxIsr(void){
    if(PIR1bits.RC1IF && PIE1bits.RC1IE) {
        if(RCSTAbits.FERR || RCSTAbits.OERR) {
            ErrorUART= true;
        } 
        else {
            rxChar = RCREG1; // Read the value in the register 
            rxFlag=true;
            switch(rxChar){
                case reloadCode:
                    reloadFlag=true;
                    break;
                case modeCode:
                    modeFlag=true;
                    break;
                case pewCode:
                    pewFlag=true;
                    break;
            }
            PIE1bits.RC1IE = 0; // Disable the interrupt control
        }
    }
}

void setUARTconfig(void){
 // configuration des PIN UART
    RPINR0_1 =   0b01100110; 
    RPOR26_27 =  0b00010001;
    // Initial register configuration for UART
    BAUDCON1 = 0x00;    // BRG16 = 0
    TXSTA1 = 0x26;      // Binary : 0010 0110 BRGH = 1 SYC = 0
    RCSTA1 = 0x90;      // Binary : 1001 0000 
    SPBRG1 = 0x19;      // Decimal: 25 = 19.23K baude
   
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
    Target[1] = rand() % 2;
}


void setPinConfig(void){
    // set pin output for each led and target
    ANCON1 = 0x00;
    TRISA = TRISA & 0b11111000; // make RA0 RA1 RA2 as output    
    LATA = 0;
    // set interrupt pin for targets 
}

void activateTarget(short targetNbr){
     //open right target
    LATA=targetNbr;
}       // still needs to be implemented

void activateLEDTarget(short targetLED[2]){
    switch (targetLED[0]){
        
        case 1:
            if (targetLED[1]){
                                    // Target 1 Mode 1
            }
            else{
                                    // Target 1 Mode 2
            } 
            break;
            
        case 2: 
            if (targetLED[1]){
                                    // Target 2 Mode 1
            }
            else{
                                    // Target 2 Mode 2
            } 
            break;
            
        case 3: 
            if (targetLED[1]){
                                    // Target 3 Mode 1
            }
            else{
                                    // Target 3 Mode 2
            } 
            break;
            
        case 4: 
            if (targetLED[1]){
                                    // Target 4 Mode 1
            }
            else{
                                    // Target 4 Mode 2
            }
            break;
            
        case 5: 
            if (targetLED[1]){
                                    // Target 5 Mode 1
            }
            else{
                                    // Target 5 Mode 2
            }
            break;
            
        case 6: 
            if (targetLED[1]){
                                    // Target 6 Mode 1
            }
            else{
                                    // Target 6 Mode 2
            }
            break;
            
    }        
} // still needs to be implemented

void fireShot(){
   
    // activate laser
}                           // still needs to be implemented

void stopShot(){
    //stop laser
}                           // still needs to be implemented