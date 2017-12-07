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

#define HUM_PITCH_1 50
#define HUM_PITCH_2 100
#define HUM_PITCH_3 150
#define HUM_PITCH_4 200
#define HUM_PITCH_5 1500
#define HUM_PITCH_6 2800
#define HUM_PITCH_7 4100
#define HUM_PITCH_8 5400
#define HUM_PITCH_9 6700

#define BAND_ANALYSIS_LENGTH 11

#define ACC_ANALYSIS_SIZE 128
#define ACC_BUFF_SIZE 2*ACC_ANALYSIS_SIZE

#endif /* INCLUDES_CONSTANTS_H_ */
