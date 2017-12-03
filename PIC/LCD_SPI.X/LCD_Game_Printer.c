#include "LCD_Game_Printer.h"

void printMBED(){
    moveCursor(TITLE_ROW,0);
    putStringLCD("  EMBEDDED UNIVERSE");
    waitASec();
    waitASec();
    clearDisplay();
}

void printStartGame(){
    moveCursor(TITLE_ROW,TITLE_COL);
    putStringLCD("S  T  A  R  T !");
    waitASec();
    waitASec();
    clearDisplay();
    putStringLCD("POINTS BULLETS TIME");
}

void printRemBullets(unsigned int nbBullets){
   moveCursor(STATS_ROW,BULLETS_COL);
   printStat(nbBullets);
}

void printRemTime(unsigned int nbSeconds){
   moveCursor(STATS_ROW,TIME_COL);
   printStat(nbSeconds);
}

void printPoints(unsigned int nbPoints){
   moveCursor(STATS_ROW,POINTS_COL);
   printStat(nbPoints);
}

void printStat(unsigned int stat){
   unsigned char statArray[3];
   sprintf(statArray, "%d", stat);
   
   if(stat < 100 && stat >= 10){
        putStringLCD(" ");
   }
   else if(stat < 10){
        putStringLCD("  ");
   }
   
   putStringLCD(statArray);
}

void printEndGame(){
    clearDisplay();
    moveCursor(TITLE_ROW,0);
    putStringLCD("E  N  D   G  A  M  E ");
}

void waitASec(){
    for(int i = 0; i < 20; i++)
        __delay_ms(50);
}

