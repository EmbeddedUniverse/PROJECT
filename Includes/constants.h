/*
 * constants.h
 *
 *  Created on: 2 nov. 2017
 *      Author: dene2303
 */

#ifndef INCLUDES_CONSTANTS_H_
#define INCLUDES_CONSTANTS_H_

#define CLK_SRC 225000000.0
#define END_OF_TRANSMISSION 4

#define IN_ASM 0

#define SHAKE_THRESHOLD 95.0

#define VOICE_BUFFER_LENGTH 8192

#define DETECTION_BUFFER_LENGTH 64
#define DETECTION_THRESHOLD 20000

/*
Frequency Definitions
#define DSK6713_AIC23_FREQ_8KHZ       1
#define DSK6713_AIC23_FREQ_16KHZ      2
#define DSK6713_AIC23_FREQ_24KHZ      3
#define DSK6713_AIC23_FREQ_32KHZ      4
#define DSK6713_AIC23_FREQ_44KHZ      5
#define DSK6713_AIC23_FREQ_48KHZ      6
#define DSK6713_AIC23_FREQ_96KHZ      7
 */

#define VOICE_AIC_SAMPLING_FREQ DSK6713_AIC23_FREQ_16KHZ
#define VOICE_SAMPLING_FREQ 16000

#define FFT_BLOCK_SIZE 512
#define FFT_BLOCK_OVERLAP 100

#define HUM_PITCH_1 200
#define HUM_PITCH_2 400
#define HUM_PITCH_3 600
#define HUM_PITCH_4 800
#define HUM_PITCH_5 2000
#define HUM_PITCH_6 3200
#define HUM_PITCH_7 4400
#define HUM_PITCH_8 5600
#define HUM_PITCH_9 6800

#endif /* INCLUDES_CONSTANTS_H_ */
