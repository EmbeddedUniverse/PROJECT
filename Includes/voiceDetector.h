#ifndef INCLUDES_VOICEDETECTOR_H_
#define INCLUDES_VOICEDETECTOR_H_

#include <stdbool.h>

#include "constants.h"

//  Set EXTERN macro :
#ifdef VOICE_DETECT_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

EXTERN volatile bool voiceSampleReady;
EXTERN volatile bool readyToPlay;
EXTERN short voiceSample[VOICE_BUFFER_LENGTH];

int VOICE_init();
int VOICE_reset();
int play(volatile short signal[], size_t length);


#undef VOICE_DETECT_MODULE_IMPORT
#undef EXTERN
#endif
