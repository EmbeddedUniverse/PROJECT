/*
 * run.c
 *
 *  Created on: 26 oct. 2017
 *      Author: para2709
 */

#include <stdio.h>
#include <dsk6713.h>
#include <dsk6713_led.h>
#include "communication.h"

void run(){


    DSK6713_init();
    COM_init();


    while(1)
    {
        unsigned char command;

        printf("Type hex code to send:\n");

        scanf("%x", &command);

        COM_send(command, PIC);
    }
}

