#include "LCD_Game_Printer.h" 

 void main(void)
 {
     unsigned char status = 0;
     unsigned char data[5];
     int i;
 
     // run internal oscillator at 8 MHz

     
     initialisation_LCD();
     printMBED();
     //OpenSPI(SPI_FOSC_16, MODE_00, SMPMID); //open SPI1
     
     //Test LCD  
     unsigned char points = 0;
     unsigned char bullets = 20;
     unsigned char time = 120;
     
     printStartGame();
     
     while (time > 0)
     {
         waitASec();
         printPoints(points++);
         printRemBullets(bullets--);
         printRemTime(time--);
         
         if(points > 255)
             points = 255;
         if(bullets == 0)
             points = 20;
     }
     
     printEndGame();
 }
 
