/*
 * File:   LCD_SPI.c
 * Author: Guacamole
 *
 * Created on November 21, 2017, 7:46 AM
 */


#include "LCD_SPI.h"

 unsigned char spi_Send_Read(unsigned char byte)
 {
     SSP2BUF = byte;    
     while(!SSP2STATbits.BF);  //While the buffer hasn't been filled
     return SSP2BUF;
 }
 
 void clearDisplay()
 {
    while(readBusyFlag());
    // Clear Display
    SPI_CS = 0;            //CSN low
    spi_Send_Read(0xF8); // Send Clear display Command
    spi_Send_Read(0x80); 
    spi_Send_Read(0x00);
    SPI_CS = 1;            //CSN high
     
 }
 
 void displayCtrl(char display, char cursor, char blink)
 {
    char BCD = blink;
    BCD <<= 1;
    BCD += cursor;
    BCD <<= 1;
    BCD += display;
    BCD <<= 1;
    BCD += 1;
    BCD <<= 4;
    
    while(readBusyFlag());
    SPI_CS = 0;            //CSN low
    spi_Send_Read(0xF8);
    spi_Send_Read(BCD);
    spi_Send_Read(0x00);
    SPI_CS = 1;            //CSN high
 }
 void putStringLCD(const unsigned char* input)
 {
    int i;
    for (i = 0; input[i] != '\0'; i++) 
    {
        putchLCD(input[i]);
    }

 }
 void putchLCD(unsigned char input)
 {
     input = fliplr(input); // LCD expects LSB first, but we send MSB first
    
    while(readBusyFlag());
    SPI_CS = 0;            //CSN low
    spi_Send_Read(0xFA); // Send write command
    spi_Send_Read(input & 0xF0); 
    spi_Send_Read(input << 4);
    SPI_CS = 1;            //CSN high
   
 }
 char fliplr(char input)
 {
     //Flips the char so it becomes LSB first
    input = (input & 0xF0) >> 4 | (input & 0x0F) << 4;
    input = (input & 0xCC) >> 2 | (input & 0x33) << 2;
    input = (input & 0xAA) >> 1 | (input & 0x55) << 1;
    
    return input;
 }
 void moveCursor(int row, int col)
 {
    char address = row * 20 + col;
    if (row >= 2)
    {
        address += 24;
    }
    address = fliplr(address);
    while(readBusyFlag());
    SPI_CS = 0;
    spi_Send_Read(0xF8); // Send 5-bit start then 000
    spi_Send_Read(address & 0xF0); 
    spi_Send_Read((address << 4) | 0x10); // D7 = 1, for Set DDRAM address command
 }
 
 unsigned char readBusyFlag()
 {
        unsigned char retValue;
             //write H on LCD register
        SPI_CS = 0;            //CSN low
        spi_Send_Read(0xFC); // Send write command
        retValue = spi_Send_Read(0x00); 
        SPI_CS = 1;            //CSN high
        __delay_us(1);
        return (retValue & 1);
 }
 void dly(void)
 {
     unsigned int i;
 
     for (i = 0; i < 50000; i++)
         ;
 }    
 
 void initialisation_SPI(void)
 {
    // PPS-Lite configuration of GPIO pins to SPI peripherial
    //RPOR24_25<3:0> = 0x4    --> RP24 = SDO2 (output)
    RPOR24_25 |= 0x04; 
    RPOR24_25 &= 0xF4;
    //RPOR22_23<3:0> = 0x4    --> RP22 = SCK2 (output)
    RPOR22_23 |= 0x05;
    RPOR22_23 &= 0xF5;
    //RPINR12_13<7:4> = 0x5    --> RP21 = SDI2 (input)
    RPINR12_13 |= 0x50;
    RPINR12_13 &= 0x5F;
    
    // SPI peripherial configuration
    SSP2CON1 &= 0xF1; // 0bxx11 0001
    SSP2CON1 |= 0x31; // 0bxx11 0001
    SSP2STAT |= 0x80;  //0b10xx xxxx
    SSP2STAT &= ~0x40;  //0b10xx xxxx
    SSP2CON3 &= 0x8F;  //0bx000 xxxx
   
 }
 
 void initialisation_PORT(void)
 {
    OSCCON = OSCCON | 0b01110000; 
     TRISDbits.TRISD4 = 0;      // SDO2 output
     TRISDbits.TRISD2 = 0;      // SCK2 output
     TRISDbits.TRISD3 = 0;      // CS output
     TRISDbits.TRISD1 = 1;      // SDI2 input
     TRISCbits.TRISC0 = 0;      //LED output
     SPI_CS = 1;        // CSN high
     Nop();
     __delay_ms(33); //wait 100 ms for the LCD to power itself on properly
     __delay_ms(33); //wait 100 ms for the LCD to power itself on properly
     __delay_ms(33); //wait 100 ms for the LCD to power itself on properly
 }

void initialisation_LCD()
{
    initialisation_PORT();
    initialisation_SPI();
    clearDisplay();
    displayCtrl(1,1,0);
    
}