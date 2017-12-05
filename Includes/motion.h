#ifndef INCLUDES_MOTION_H_
#define INCLUDES_MOTION_H_

#include <stdbool.h>

void MOTION_init();
bool MOTION_readyToAnalyze();
bool MOTION_detectReload();
bool MOTION_detectModeChange();

#endif /* INCLUDES_MOTION_H_ */
