#ifndef INCLUDES_PIOURECOGNITION_H_
#define INCLUDES_PIOURECOGNITION_H_

#include "constants.h"
#include "stdbool.h"

//  Set EXTERN macro :
#ifdef PIOU_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

bool detectPiou(short sample[VOICE_BUFFER_LENGTH]);
void PIOU_init();

#undef PIOU_MODULE_IMPORT
#undef EXTERN
#endif
