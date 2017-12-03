#ifndef INCLUDES_COMMUNICATION_H_
#define INCLUDES_COMMUNICATION_H_

#define ACCEL_MAX3111 0
#define PIC_MAX3111 1

typedef enum
{
    ACCEL = ACCEL_MAX3111,
    PIC = PIC_MAX3111,
    DEFAULT = 0
} SPI_Interface;


int COM_init();
int COM_selectInterface(SPI_Interface interface);

#endif /* INCLUDES_COMMUNICATION_H_ */
