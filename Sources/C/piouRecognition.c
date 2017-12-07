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

#define ERROR_TOLERANCE_T 0.10
#define ERROR_TOLERANCE_A 0.05

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
bool isP(int cblock);
bool isI(int cblock);
bool isU(int cblock);
bool isT(int cblock);
bool isA(int cblock);

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
        bands[cblock][4] = 0.0;
        bands[cblock][5] = 0.0;
        bands[cblock][6] = 0.0;
        bands[cblock][7] = 0.0;
        bands[cblock][8] = 0.0;
        bands[cblock][9] = 0.0;
        bands[cblock][10] = 0.0;

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

        bands[cblock][0] = bands[cblock][1] + bands[cblock][2] +
                            bands[cblock][3] + bands[cblock][4] +
                            bands[cblock][5] + bands[cblock][6] +
                            bands[cblock][7] + bands[cblock][8] +
                            bands[cblock][9] + bands[cblock][10];

        bands[cblock][1] /= bands[cblock][0];
        bands[cblock][2] /= bands[cblock][0];
        bands[cblock][3] /= bands[cblock][0];
        bands[cblock][4] /= bands[cblock][0];
        bands[cblock][5] /= bands[cblock][0];
        bands[cblock][6] /= bands[cblock][0];
        bands[cblock][7] /= bands[cblock][0];
        bands[cblock][8] /= bands[cblock][0];
        bands[cblock][9] /= bands[cblock][0];
        bands[cblock][10] /= bands[cblock][0];

        //printf("%f\t%f\t%f\t%f\t\r\n", bands[cblock][0], bands[cblock][1], bands[cblock][2], bands[cblock][3]);

//        if(bands[cblock][0] < 0.10*bands[0][0])
//            detectedPhonems[cblock] = '0';
//        else if (isP(cblock))
//            detectedPhonems[cblock] = 'p';
//        else if (isI(cblock) || isU(cblock))
//        {
//            if (isI(cblock))
//                detectedPhonems[cblock] = 'i';
//            else
//                detectedPhonems[cblock] = 'u';
//        }
//        else
//            detectedPhonems[cblock] = 'p';


//        if(bands[cblock][0] < 0.10*bands[0][0])
//            detectedPhonems[cblock] = '0';
//        else if(isU(cblock))
//            detectedPhonems[cblock] = 'u';
//        else if(isI(cblock))
//            detectedPhonems[cblock] = 'i';
//        else if(isP(cblock))
//            detectedPhonems[cblock] = 'p';
//        else
//            detectedPhonems[cblock] = '0';

        if(bands[cblock][0] < 0.10*bands[0][0])
            detectedPhonems[cblock] = '0';
        else if(isT(cblock))
            detectedPhonems[cblock] = 'T';
        else if(isA(cblock))
            detectedPhonems[cblock] = 'A';
        else
            detectedPhonems[cblock] = 'X';

        start += FFT_BLOCK_SIZE - FFT_BLOCK_OVERLAP;
    }

    int o;
    float tabTemp[10];

    for(o = 0; o <NB_BLOCKS; o++){
        tabTemp[0]=bands[o][1];
        tabTemp[1]=bands[o][2];
        tabTemp[2]=bands[o][3];
        tabTemp[3]=bands[o][4];
        tabTemp[4]=bands[o][5];
        tabTemp[5]=bands[o][6];
        tabTemp[6]=bands[o][7];
        tabTemp[7]=bands[o][8];
        tabTemp[8]=bands[o][9];
        tabTemp[9]=bands[o][10];
    }



    return goodSeries();
}

// Determines if the series of phonem is a pew
bool goodSeries()
{
//    int ipIndex = 0;
//
//    // There must be a P in the first 3 phonems
//    for(; ipIndex <= 3; ++ipIndex)
//    {
//        if (ipIndex >= 3)
//            return false;
//
//        if (detectedPhonems[ipIndex] == 'p')
//            break;
//    }
//
//    // There make sure there are no more than 3 consecutive P
//    int nbP = 0;
//    while(detectedPhonems[ipIndex] == 'p')
//    {
//        ++nbP;
//        ++ipIndex;
//        if (nbP > 3)
//            return false;
//    }
//
//    // Allowing one mistake from P to i transition
//    if(detectedPhonems[ipIndex] != 'i')
//        if(detectedPhonems[++ipIndex] != 'i')
//            return false;
//
//    // Allowing up to 4 consecutive i
//    int nbI = 0;
//    while(detectedPhonems[ipIndex] == 'i')
//    {
//        ++nbI;
//        ++ipIndex;
//        if (nbI > 3)
//            return false;
//    }
//
//    // Allowing one mistake between i an u
//    if(detectedPhonems[ipIndex] != 'u')
//        if(detectedPhonems[++ipIndex] != 'u')
//            return false;
//
//    // Allowing up to 8 u
//    int nbU = 0;
//    while(detectedPhonems[ipIndex] == 'u')
//    {
//        ++nbU;
//        ++ipIndex;
//        if (nbU > 7)
//            return false;
//    }

    int index = 0;

    // There must be a A in 2 phonems
    for(; index < NB_BLOCKS-3; ++index)    {
        if(detectedPhonems[index] == 'T')
            if(detectedPhonems[index+1] == 'A' || detectedPhonems[index+2] == 'A' || detectedPhonems[index+3] == 'A')
                        return true;
    }

    return false;
}

bool isP(int cblock){
    if(     bands[cblock][1] > (0.152039) &&
            bands[cblock][1] < (0.395098) &&
            bands[cblock][2] > (0.080106) &&
            bands[cblock][2] < (0.272798) &&
            bands[cblock][3] > (0.028439) &&
            bands[cblock][3] < (0.065082) &&
            bands[cblock][4] > (0.025390) &&
            bands[cblock][4] < (0.039798) &&
            bands[cblock][5] > (0.029348) &&
            bands[cblock][5] < (0.072512) &&
            bands[cblock][6] > (0.033610) &&
            bands[cblock][6] < (0.122305) &&
            bands[cblock][7] > (0.058029) &&
            bands[cblock][7] < (0.127883) &&
            bands[cblock][8] > (0.044471) &&
            bands[cblock][8] < (0.124299) &&
            bands[cblock][9] > (0.044722) &&
            bands[cblock][9] < (0.135231) &&
            bands[cblock][10] > (0.026882) &&
            bands[cblock][10] < (0.121957))
        return true;
    else
        return false;
}

bool isI(int cblock){
    if(     bands[cblock][1] > (0.157429) && bands[cblock][1] < (0.488406) &&
            bands[cblock][2] > (0.080848) && bands[cblock][2] < (0.360737) &&
            bands[cblock][3] < (0.109583) &&
            bands[cblock][4] < (0.079256) &&
            bands[cblock][5] < (0.121856) &&
            bands[cblock][6] < (0.120087) &&
            bands[cblock][7] < (0.074192) &&
            bands[cblock][8] < (0.062410) &&
            bands[cblock][9] < (0.059454) &&
            bands[cblock][10] < (0.041437))
        return true;
    else
        return false;
}

bool isU(int cblock){
    if(     bands[cblock][1] > (0.288864) && bands[cblock][1] < (0.614346) &&
            bands[cblock][2] > (0.138250) && bands[cblock][2] < (0.385840) &&
            bands[cblock][3] < (0.165023) &&
            bands[cblock][4] < (0.079977) &&
            bands[cblock][5] < (0.059757) &&
            bands[cblock][6] < (0.047494) &&
            bands[cblock][7] < (0.025625) &&
            bands[cblock][8] < (0.024093) &&
            bands[cblock][9] < (0.019580) &&
            bands[cblock][10] < (0.014740))
        return true;
    else
        return false;
}

//bool isTA(int cblock){
//    if(     bands[cblock][1] > (0.24980376) && bands[cblock][1] < (0.56989167) &&
//            bands[cblock][2] > (0.02752650) && bands[cblock][2] < (0.23679538) &&
//            bands[cblock][3] < (0.08720931) &&
//            bands[cblock][4] < (0.12465798) &&
//            bands[cblock][5] < (0.10428346) &&
//            bands[cblock][6] < (0.05677224) &&
//            bands[cblock][7] < (0.06651647) &&
//            bands[cblock][8] < (0.08091772) &&
//            bands[cblock][9] < (0.08169756) &&
//            bands[cblock][10] < (0.08042542))
//        return true;
//    else
//        return false;
//}


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
