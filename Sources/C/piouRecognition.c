#include <math.h>
#include <DSPF_sp_cfftr2_dit.h>
#include <DSPF_sp_bitrev_cplx.h>
#include "twiddles512.h"
#include "bitrev_index.h"

#define PIOU_MODULE_IMPORT
#include "piouRecognition.h"

#define FIR_FILTER 0

#if (FIR_FILTER)
#define FIR_K -0.5
#endif

#define HUM_PITCH_INDEX_1 FFT_BLOCK_SIZE*HUM_PITCH_1/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_2 FFT_BLOCK_SIZE*HUM_PITCH_2/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_3 FFT_BLOCK_SIZE*HUM_PITCH_3/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_4 FFT_BLOCK_SIZE*HUM_PITCH_4/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_5 FFT_BLOCK_SIZE*HUM_PITCH_5/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_6 FFT_BLOCK_SIZE*HUM_PITCH_6/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_7 FFT_BLOCK_SIZE*HUM_PITCH_7/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_8 FFT_BLOCK_SIZE*HUM_PITCH_8/VOICE_SAMPLING_FREQ
#define HUM_PITCH_INDEX_9 FFT_BLOCK_SIZE*HUM_PITCH_9/VOICE_SAMPLING_FREQ
#define NB_BLOCKS  (VOICE_BUFFER_LENGTH/(FFT_BLOCK_SIZE-FFT_BLOCK_OVERLAP))-1

#define ERROR_TOLERANCE_T 0.08
#define ERROR_TOLERANCE_A 0.04

float HAM_WINDOW[FFT_BLOCK_SIZE];

char detectedPhonems[NB_BLOCKS];
#pragma DATA_ALIGN(analysisBlock, FFT_BLOCK_SIZE*4)
float analysisBlock [FFT_BLOCK_SIZE*2];
float harmonicAmplitudes[FFT_BLOCK_SIZE/2];
float bands [NB_BLOCKS][11];
short indeces[FFT_BLOCK_SIZE/16];

double sumSquareErrorT[NB_BLOCKS];
double sumSquareErrorA[NB_BLOCKS];

bool goodSeries();
bool isT(int cblock);
bool isA(int cblock);

void PIOU_init(){
    // Compute bit reverse indeces
    bitrev_index(indeces, FFT_BLOCK_SIZE);

    // Fill hamming window
    int n;
    for (n = 0; n < FFT_BLOCK_SIZE; ++n)
        HAM_WINDOW[n] = 0.54-0.46*cos(6.28318530718 * (float)n / (float)(FFT_BLOCK_SIZE-1));
}

bool detectPiou(short sample[VOICE_BUFFER_LENGTH]){
    int cblock, i;
    int start = 0;

    for(cblock = 0; cblock < NB_BLOCKS; ++cblock){
        for (i = 0; i < FFT_BLOCK_SIZE; ++i){
            analysisBlock[i*2] = HAM_WINDOW[i] * ((float)sample[start+i])/(1<<13);
            analysisBlock[i*2+1] = 0;
        }

        // Perform FFT on analysis Block
        DSPF_sp_cfftr2_dit((float*)analysisBlock, (float*)w512, FFT_BLOCK_SIZE);
        DSPF_sp_bitrev_cplx((double*)analysisBlock, indeces, FFT_BLOCK_SIZE);

        for(i = 0; i < FFT_BLOCK_SIZE / 2; ++i)
            harmonicAmplitudes[i] = sqrt(analysisBlock[2*i]*analysisBlock[2*i] + analysisBlock[2*i+1]*analysisBlock[2*i+1]);

        for(i = 1; i < BAND_ANALYSIS_LENGTH; i++)
            bands[cblock][i] = 0.0;

        for (i = 0; i < HUM_PITCH_INDEX_1; ++i)
            bands[cblock][1] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_1; i < HUM_PITCH_INDEX_2; ++i)
            bands[cblock][2] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_2; i < HUM_PITCH_INDEX_3; ++i)
             bands[cblock][3] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_3; i < HUM_PITCH_INDEX_4; ++i)
             bands[cblock][4] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_4; i < HUM_PITCH_INDEX_5; ++i)
             bands[cblock][5] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_5; i < HUM_PITCH_INDEX_6; ++i)
             bands[cblock][6] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_6; i < HUM_PITCH_INDEX_7; ++i)
             bands[cblock][7] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_7; i < HUM_PITCH_INDEX_8; ++i)
             bands[cblock][8] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_8; i < HUM_PITCH_INDEX_9; ++i)
             bands[cblock][9] += harmonicAmplitudes[i];
        for (i = HUM_PITCH_INDEX_9; i < FFT_BLOCK_SIZE / 2; ++i)
             bands[cblock][10] += harmonicAmplitudes[i];

        bands[cblock][0] = 0;
        for(i = 1; i < BAND_ANALYSIS_LENGTH; i++)
            bands[cblock][0] += bands[cblock][i];

        for(i = 1; i < BAND_ANALYSIS_LENGTH; i++)
            bands[cblock][i] /= bands[cblock][0];

        isA(cblock);
        isT(cblock);

        if( sumSquareErrorA[cblock] > ERROR_TOLERANCE_A &&
            sumSquareErrorT[cblock] > ERROR_TOLERANCE_T)
            detectedPhonems[cblock] = 'X';
        else if(sumSquareErrorA[cblock] < sumSquareErrorT[cblock])
            detectedPhonems[cblock] = 'A';
        else
            detectedPhonems[cblock] = 'T';

        if(bands[cblock][0] < 0.10*bands[0][0])
            detectedPhonems[cblock] = '0';

        start += FFT_BLOCK_SIZE - FFT_BLOCK_OVERLAP;
    }

    return goodSeries();
}

// Determines if the series of phonem is a pew
bool goodSeries(){
    int index = 0;

    //Condition to declare that there's a 'TA'
    for(; index < NB_BLOCKS-3; ++index)    {
        if(detectedPhonems[index] == 'T')
            if(detectedPhonems[index+1] == 'A' || detectedPhonems[index+2] == 'A' || detectedPhonems[index+3] == 'A')
                        return true;
    }

    return false;
}

bool isT(int cblock){
    sumSquareErrorT[cblock] = ((0.067752-bands[cblock][1])*(0.067752-bands[cblock][1]) +
                            (0.100295-bands[cblock][2])*(0.100295-bands[cblock][2]) +
                            (0.028310-bands[cblock][3])*(0.028310-bands[cblock][3]) +
                            (0.040523-bands[cblock][4])*(0.040523-bands[cblock][4]) +
                            (0.216980-bands[cblock][5])*(0.216980-bands[cblock][5]) +
                            (0.119700-bands[cblock][6])*(0.119700-bands[cblock][6]) +
                            (0.111312-bands[cblock][7])*(0.111312-bands[cblock][7]) +
                            (0.113067-bands[cblock][8])*(0.113067-bands[cblock][8]) +
                            (0.112665-bands[cblock][9])*(0.112665-bands[cblock][9]) +
                            (0.089396-bands[cblock][10])*(0.089396-bands[cblock][10]));

    if( sumSquareErrorT[cblock] < ERROR_TOLERANCE_T )
        return true;
    else
        return false;
}

bool isA(int cblock){
    sumSquareErrorA[cblock] = ((0.056759-bands[cblock][1])*(0.056759-bands[cblock][1]) +
                            (0.077402-bands[cblock][2])*(0.077402-bands[cblock][2]) +
                            (0.026613-bands[cblock][3])*(0.026613-bands[cblock][3]) +
                            (0.048531-bands[cblock][4])*(0.048531-bands[cblock][4]) +
                            (0.299821-bands[cblock][5])*(0.299821-bands[cblock][5]) +
                            (0.112100-bands[cblock][6])*(0.112100-bands[cblock][6]) +
                            (0.086246-bands[cblock][7])*(0.086246-bands[cblock][7]) +
                            (0.092258-bands[cblock][8])*(0.092258-bands[cblock][8]) +
                            (0.103228-bands[cblock][9])*(0.103228-bands[cblock][9]) +
                            (0.097041-bands[cblock][10])*(0.097041-bands[cblock][10]));

    if( sumSquareErrorA[cblock] < ERROR_TOLERANCE_A && bands[cblock][5] > 0.20 )
        return true;
    else
        return false;
}
