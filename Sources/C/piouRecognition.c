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

#define HUM_LOWPITCH_INDEX FFT_BLOCK_SIZE*HUM_LOWPITCH/VOICE_SAMPLING_FREQ
#define HUM_HIGHPITCH_INDEX FFT_BLOCK_SIZE*HUM_HIGHPITCH/VOICE_SAMPLING_FREQ
#define NB_BLOCKS  (VOICE_BUFFER_LENGTH/(FFT_BLOCK_SIZE-FFT_BLOCK_OVERLAP))-1

float HAM_WINDOW[FFT_BLOCK_SIZE];

char detectedPhonems[NB_BLOCKS];
#pragma DATA_ALIGN(analysisBlock, FFT_BLOCK_SIZE*4)
float analysisBlock [FFT_BLOCK_SIZE*2];
float harmonicAmplitudes[FFT_BLOCK_SIZE/2];
float bands [NB_BLOCKS][4];
short indeces[FFT_BLOCK_SIZE/16];

bool goodSeries();

void PIOU_init()
{
    // Compute bit reverse indeces
    bitrev_index(indeces, FFT_BLOCK_SIZE);

    // Fill hamming window
    int n;
    for (n = 0; n < FFT_BLOCK_SIZE; ++n)
        HAM_WINDOW[n] = 0.54-0.46*cos(6.28318530718 * (float)n / (float)(FFT_BLOCK_SIZE-1));

    // Compute harmonic indeces
}

bool detectPiou(short sample[VOICE_BUFFER_LENGTH])
{
    int cblock, i;
    int start = 0;
    for(cblock = 0; cblock < NB_BLOCKS; ++cblock)
    {
        //printf("\r\n");

        for (i = 0; i < FFT_BLOCK_SIZE; ++i)
        {
            analysisBlock[i*2] = HAM_WINDOW[i] * ((float)sample[start+i])/(1<<13);
            analysisBlock[i*2+1] = 0;
        }

        // Perform FFT on analysis Block
        DSPF_sp_cfftr2_dit((float*)analysisBlock, (float*)w512, FFT_BLOCK_SIZE);
        DSPF_sp_bitrev_cplx((double*)analysisBlock, indeces, FFT_BLOCK_SIZE);

        for(i = 0; i < FFT_BLOCK_SIZE / 2; ++i)
        {
            harmonicAmplitudes[i] = sqrt(analysisBlock[2*i]*analysisBlock[2*i] + analysisBlock[2*i+1]*analysisBlock[2*i+1]);
        }

        bands[cblock][1] = 0.0;
        bands[cblock][2] = 0.0;
        bands[cblock][3] = 0.0;
        for (i = 0; i < HUM_LOWPITCH_INDEX; ++i)
            bands[cblock][1] += harmonicAmplitudes[i];
        for (i = HUM_LOWPITCH_INDEX; i < HUM_HIGHPITCH_INDEX; ++i)
            bands[cblock][2] += harmonicAmplitudes[i];
        for (i = HUM_HIGHPITCH_INDEX; i < FFT_BLOCK_SIZE / 2; ++i)
             bands[cblock][3] += harmonicAmplitudes[i];
        bands[cblock][0] = bands[cblock][1] + bands[cblock][2] + bands[cblock][3];
        bands[cblock][1] /= bands[cblock][0];
        bands[cblock][2] /= bands[cblock][0];
        bands[cblock][3] /= bands[cblock][0];

        //printf("%f\t%f\t%f\t%f\t\r\n", bands[cblock][0], bands[cblock][1], bands[cblock][2], bands[cblock][3]);

        if(bands[cblock][0] < 0.10*bands[0][0])
            detectedPhonems[cblock] = '0';
        else if (bands[cblock][3] > 0.2)
            detectedPhonems[cblock] = 'p';
        else if (bands[cblock][1] > 0.3)
        {
            if (bands[cblock][2] > 0.2)
                detectedPhonems[cblock] = 'i';
            else
                detectedPhonems[cblock] = 'u';
        }
        else
            detectedPhonems[cblock] = 'p';

        start += FFT_BLOCK_SIZE - FFT_BLOCK_OVERLAP;
    }

    return goodSeries();
}

// Determines if the series of phonem is a pew
bool goodSeries()
{
    int ipIndex = 0;

    // There must be a P in the first 3 phonems
    for(; ipIndex <= 3; ++ipIndex)
    {
        if (ipIndex >= 3)
            return false;

        if (detectedPhonems[ipIndex] == 'p')
            break;
    }

    // There make sure there are no more than 3 consecutive P
    int nbP = 0;
    while(detectedPhonems[ipIndex] == 'p')
    {
        ++nbP;
        ++ipIndex;
        if (nbP > 3)
            return false;
    }

    // Allowing one mistake from P to i transition
    if(detectedPhonems[ipIndex] != 'i')
        if(detectedPhonems[++ipIndex] != 'i')
            return false;

    // Allowing up to 4 consecutive i
    int nbI = 0;
    while(detectedPhonems[ipIndex] == 'i')
    {
        ++nbI;
        ++ipIndex;
        if (nbI > 3)
            return false;
    }

    // Allowing one mistake between i an u
    if(detectedPhonems[ipIndex] != 'u')
        if(detectedPhonems[++ipIndex] != 'u')
            return false;

    // Allowing up to 8 u
    int nbU = 0;
    while(detectedPhonems[ipIndex] == 'u')
    {
        ++nbU;
        ++ipIndex;
        if (nbU > 7)
            return false;
    }


    return true;
}
