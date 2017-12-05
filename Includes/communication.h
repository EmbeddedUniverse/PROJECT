#ifndef INCLUDES_COMMUNICATION_H_
#define INCLUDES_COMMUNICATION_H_

#include "stdbool.h"

#define MAX3111_DEVICE_COUNT 2
#define ACCEL_MAX3111 0
#define PIC_MAX3111 1

typedef enum
{
    ACCEL = ACCEL_MAX3111,
    PIC = PIC_MAX3111,
    DEFAULT = 0
} SPI_Interface;

// Callback function type
typedef void (*ReceiveCallBack) (unsigned char);

int COM_init();
int COM_selectInterface(SPI_Interface interface);
int COM_send(unsigned char data, SPI_Interface interface);
void COM_setReceiveCallBack(void (*callBack)(unsigned char), SPI_Interface interface);
void clearFIFO(SPI_Interface interface);
bool COM_syncRead(SPI_Interface interface, unsigned char *receivedValue);

#endif /* INCLUDES_COMMUNICATION_H_ */
