

#include <stdio.h>
#include <stdbool.h>
#include "communication.h"
#include "picProtocol.h"

#ifndef INCLUDES_MENU_H_
#define INCLUDES_MENU_H_



void runMenu(unsigned char *ammoCode, unsigned char *timeCode);
void choisirNbrBalle(unsigned char *ammoCode);
void choisrTemps(unsigned char *timeCode);
void instruction();



#endif /* INCLUDES_RUN_H_ */
