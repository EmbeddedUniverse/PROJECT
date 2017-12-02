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
statePIC myState = IDLE;

typedef enum gunMode{
    MODE0,
    MODE1
}gunModeState;
gunModeState myModeState = MODE0;
// Global variables


unsigned char rxChar = 0x00;
unsigned char transferedData = 0x00;
short nextTarget[2];
int ammoLeft = 12;
int toggleCounter = 0;
int globalTimer = 0;

bool ErrorUART      = false; 
bool rxFlag         = false; // USART Data Received flag
bool reloadFlag     = false;
bool modeFlag       = false; 
bool pewFlag        = false;
bool capteurFlag    = false;
bool timerFlag      = false;

// Declarations
void interrupt rxIsr(void);
void setUARTconfig(void);
void setPinConfig(void);
void setInterruptConfig();
void setTimerConfig();

void getRandomTarget(short Target[2]);
void activateTarget(short targetNbr);
void activateLEDTarget(short targetLED[2]);
void fireShot();
void stopShot();
void changeMode();
void setModeLED(gunModeState Mode);
void toggleGunLED();


// Main 
void main(void) {
    setUARTconfig();
    setPinConfig();
    setTimerConfig();
    setInterruptConfig();
    T0CONbits.TMR0ON = 1;
    
    while(1){    
        if(timerFlag){
            globalTimer++;
            timerFlag=false;
            INTCONbits.TMR0IE = 1;  //timer enable 
        }
        if(globalTimer==20 ){
            myState= END_GAME;
        } 
    }
    //getRandomTarget(nextTarget);
    //activateTarget(nextTarget[0]);
    
    
    /*while(1);
    while(true){
        if(rxFlag){
            rxChar = RCREG1; // Read the value in the register       
            rxFlag = false; // Reset the flag
            PIE1bits.RC1IE = 1; // Re-enable the interrupt control
            TXREG1 = 0xf0;
            
        }
    }*/
    
    while(1){
        
        if(timerFlag){
            globalTimer++;
            timerFlag=false;
            INTCONbits.TMR0IE = 1;  //timer enable 
        }
        
        if(globalTimer==120 ){
            myState= END_GAME;
        }
        
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
                    changeMode();
                    setModeLED(myModeState);
                    modeFlag=false;
                    PIE1bits.RC1IE = 1;
                }

                else if (pewFlag && ammoLeft !=0){
                    fireShot();
                    for (int i =0;i<10000;i++){}
                    stopShot();
                    ammoLeft -= 1;
                    pewFlag = false;
                    PIE1bits.RC1IE = 1;
                }
                else if(ammoLeft==0){
                    myState = NEED_RELOAD;
                }
                else if(capteurFlag && myModeState == nextTarget[1]){
                    capteurFlag = false;
                    INTCON3bits.INT1E = 1; 
                    myState = ACCUMULATE_POINTS;
                }

                break;

            case NEED_RELOAD:
                toggleCounter++;
                if (toggleCounter==5000){
                    toggleGunLED();
                    toggleCounter=0;
                }
                if(reloadFlag){
                    ammoLeft= 12;
                    setModeLED(myModeState);
                    toggleCounter=0;
                    reloadFlag=false;
                    PIE1bits.RC1IE = 1;
                }
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
    if(INTCON3bits.INT1IF && INTCON3bits.INT1E){
        capteurFlag= true;
        INTCON3bits.INT1E = 0;
        INTCON3bits.INT1F = 0;
    }
    
    if (INTCONbits.TMR0IF && INTCONbits.TMR0IE ){
        timerFlag = true;
        INTCONbits.TMR0IF = 0;  //timer Flag = 0
        INTCONbits.TMR0IE = 0;  //timer enable 
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
    // Send initial signal & console clear
    TXREG1 = 0xAA;
}
void setInterruptConfig(void){
// Initial interrupt configuration
    
    //timer
    INTCONbits.TMR0IF = 0;  //timer Flag = 0
    INTCONbits.TMR0IE = 1;  //timer enable 
   
    // Uart
    IPR1bits.RC1IP = 0; // Set the UART interrupt to low priority
    PIE1bits.RC1IE = 1; // EUSART Receive Interrupt Enable bit
    
    // target
    RPINR26_27 = 0xC1; // INT1  = RP5 = RA5 
    TRISAbits.TRISA5 = 1;    
    INTCON2bits.INTEDG1 = 1;    // interrupt INT1 on rising edge
    INTCON3bits.INT1F = 0;      // interrupt INT1 flag at 0
    INTCON3bits.INT1P = 0;      // interrupt INT1 on low priority
    INTCON3bits.INT1E = 1;      // Enables the INT1 external interrupt 
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;     // Asynchronous mandatory register - USART
    INTCONbits.PEIE = 1;    // Asynchronous mandatory register - USART
}
void setPinConfig(void){
    // set pin output for each led and target
    ANCON1 = 0x00;
    TRISA &= 0b11111000;    // Make RA0 RA1 RA2 as output for mux target
    TRISC &= 0b00011000;    // Make RC0-RC2 and RC5-RC& as output for mux LED
    TRISD &= 0b11011110;    // Make RD0 and RD5 as output for mode LED
    TRISBbits.TRISB1 = 0;   // Make RB1 as firing shot; 
    LATA = 0;
    // set interrupt pin for targets 
}
void setTimerConfig(void){
    T0CONbits.TMR0ON = 0;   // stop timer 
    T0CONbits.T08BIT = 0;   // timer on 16 bit;
    T0CONbits.T0CS = 0;     // internal clock; 16 kHz
    T0CONbits.PSA = 0;      // prescaler working
    T0CONbits.T0PS= 0b011;      // 1:16 prescaler for(1khz) 

}

void getRandomTarget(short Target[2]){
    Target[0] = 0;//rand() % 6;
    Target[1] = 0;//rand() % 2;
}

void changeMode(){
    if (myModeState == MODE0){
        myModeState=MODE1;                  
    }       
    else{
        myModeState=MODE0;
    }
}

void activateTarget(short targetNbr){
     //open right target
    LATA=targetNbr+2;
}     

void activateLEDTarget(short targetLED[2]){
    short ledNBR = 0;
    if (targetLED[0]<3){
        ledNBR += (2*targetLED[0]+targetLED[1]);
        ledNBR += 240;
        LATC = ledNBR;
    }
    else{  
        ledNBR += ((2*(targetLED[0]-3)+targetLED[1])<<4);
        ledNBR += 15;
        LATC = ledNBR;
    }
}
 
void fireShot(){
    LATBbits.LATB1 = 1; // activate laser
}                           

void stopShot(){
    LATBbits.LATB1 = 0; //stop laser
}                          

void setModeLED(gunModeState Mode){
    switch (Mode){
        case MODE0:
            LATDbits.LATD0 = 1;   //Open LED MODE0
            LATDbits.LATD5 = 0;   //Close LED MODE1
            break;
        case MODE1: 
            LATDbits.LATD0 = 1;   //Open LED MODE1
            LATDbits.LATD5 = 0;   //Close LED MODE0
            break;
            
    }
}

void toggleGunLED(){
    if (LATDbits.LATD0==0){
        LATDbits.LATD0 = 1;
        LATDbits.LATD5 = 1;
    }
    else{
        LATDbits.LATD0 = 0;
        LATDbits.LATD5 = 0;
    }
}