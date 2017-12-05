#include <stdio.h>
#include <dsk6713.h>
#include <dsk6713_led.h>
#include "voiceDetector.h"
#include "piouRecognition.h"
#include "communication.h"
#include "accelProtocol_UART.h"
#include "picProtocol.h"
#include "motion.h"

#define RELOAD_LED 0
#define CHANGE_MODE_LED 1
#define FIRE_LED 2

unsigned char picMessage;

void receiveFromPIC(unsigned char data)
{
    picMessage = data;
}

void run(unsigned char ammoCode, unsigned timeCode)
{
    printf("\n----------------------------------------------\n");
    printf("\t    STARTING GAME\n");
    printf("----------------------------------------------\n");

    CSL_init();
    DSK6713_init();

    DSK6713_LED_init();
    DSK6713_LED_off(RELOAD_LED);
    DSK6713_LED_off(CHANGE_MODE_LED);
    DSK6713_LED_off(FIRE_LED);

    VOICE_init();
    PIOU_init();

    COM_setReceiveCallBack(aggregateAccelBytes, ACCEL);
    COM_setReceiveCallBack(receiveFromPIC, PIC);
    COM_init();

//    COM_send(ammoCode, PIC);
//    DSK6713_waitusec(10000);
//    COM_send(timeCode, PIC);
//    DSK6713_waitusec(10000);
    COM_send(START_CODE, PIC);
    DSK6713_waitusec(1000);

    MOTION_init();

    picMessage = 0;

    clearFIFO(PIC);

    while(picMessage != END_CODE)
    {
        if (MOTION_readyToAnalyze())
        {
            if (MOTION_detectReload())
            {
                COM_send(RELOAD_CODE, PIC);
                DSK6713_LED_on(RELOAD_LED);
            }
            else
                DSK6713_LED_off(RELOAD_LED);

            if (MOTION_detectModeChange())
            {
                COM_send(MODE_CHANGE_CODE, PIC);
                DSK6713_LED_on(CHANGE_MODE_LED);
            }
            else
                DSK6713_LED_off(CHANGE_MODE_LED);
        }

        if (voiceSampleReady)
        {
            if (detectPiou(voiceSample))
            {
                COM_send(FIRE_CODE, PIC);
                DSK6713_LED_on(FIRE_LED);
            }
            else
                DSK6713_LED_off(FIRE_LED);

            VOICE_reset();
            clearFIFO(ACCEL);
        }

        //COM_syncRead(PIC, &picMessage);
    }

    printf("\n----------------------------------------------\n");
    printf("\t    GAME  OVER\n");
    printf("----------------------------------------------\n");
}

