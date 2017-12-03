/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_SPI_H
#define	LCD_SPI_H

#include <p18f67j94.h>
#include <xc.h>
 
 //function prototypes
unsigned char spi_Send_Read(unsigned char);
void initialisation_SPI(void);
void dly(void);
unsigned char readBusyFlag(void);
void putStringLCD(const unsigned char*);
void putchLCD(unsigned char);
void clearDisplay();
void displayCtrl(char display, char cursor, char blink);
void initialisation_PORT(void);
void initialisation_LCD();
void moveCursor(int row, int col);
char fliplr(char input);

 // Defines
 #define SPI_SCK        LATDbits.LATD2        // Clock pin, PORTD pin 2 
 #define SPI_SO         LATDbits.LATD4        // Serial output pin, PORTD pin 4 
 #define SPI_CS         LATDbits.LATD3        // CSN output pin, PORTD pin 3
 #define LED            LATCbits.LATC0        //LED output pin, PORTC pin 0 

 
#define _XTAL_FREQ 8000000

 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LCD_SPI_H */

