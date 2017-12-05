/*
 * Defines the byte code specified for the communication between PIC and DSK
 */

#ifndef INCLUDES_PICPROTOCOL_H_
#define INCLUDES_PICPROTOCOL_H_

#define RELOAD_CODE         0xAA
#define MODE_CHANGE_CODE    0xBB
#define FIRE_CODE           0xCC
#define START_CODE          0xDD

#define _6Ammo              0x0A
#define _12Ammo             0x0B
#define _24Ammo             0x0C

#define _30secTimer         0xA0
#define _60secTimer         0xB0
#define _120secTimer        0xC0

#endif /* INCLUDES_PICPROTOCOL_H_ */
