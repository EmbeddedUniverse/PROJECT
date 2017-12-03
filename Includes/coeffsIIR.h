

#ifndef INCLUDES_COEFFSIIR_H_
#define INCLUDES_COEFFSIIR_H_
//**********ELLIPTIC FILTERS **********
//	Order = 8
//	Representation in 2Q13
#define nbFilters 5
#define nbSecondOrder 4
#define nbCoeffs 6

//Second Order Filters Structure
typedef struct {
	const short globalGain[nbSecondOrder];
	const short coeffsIIR[nbSecondOrder][nbCoeffs];} secondOrdfilters; 

//FILTER 1 (Bandpass: 600 to 900Hz)
const secondOrdfilters sOFilters1 =  {{54}, {{ 8192, -9081, 8192, 8192, -12455, 5608},
                                             { 8192, -16333, 8192, 8192, -14901, 6971},
                                             { 8192, -14402, 8192, 8192, -15121, 7861},
                                             { 8192, -16158, 8192, 8192, -15670, 7953}}};

//FILTER 2 (Bandpass: 3100 to 3900Hz)
const secondOrdfilters sOFilters2 =  {{96}, {{ 8192, 12866, 8192, 8192, 3233, 3367},
                                             { 8192, -14705, 8192, 8192, -7244, 3954},
                                             { 8192, 4952, 8192, 8192, -1100, 7246},
                                             { 8192, -10029, 8192, 8192, -4917, 7290}}};

//FILTER 3 (Bandpass: 1500 to 1800Hz)
const secondOrdfilters sOFilters3 =  {{59}, {{ 8192, 1650, 8192, 8192, -5905, 3703},
                                             { 8192, -16052, 8192, 8192, -13091, 5911},
                                             { 8192, -9321, 8192, 8192, -12200, 7578},
                                             { 8192, -14956, 8192, 8192, -13001, 7637}}};

//FILTER 4 (Bandpass: 4200 to 4700Hz)
const secondOrdfilters sOFilters4 =  {{80}, {{ 8192, -12611, 8192, 8192, -4123, 3456},
                                             { 8192, 14421, 8192, 8192, 7592, 4029},
                                             { 8192, -4359, 8192, 8192, 1702, 7249},
                                             { 8192, 9139, 8192, 8192, 3734, 7270}}};

//FILTER 5 (Bandpass: 6800 to 7800Hz)
const secondOrdfilters sOFilters5 =  {{75}, {{ 8192, 5198, 8192, 8192, 14335, 6602},
                                             { 8192, 16378, 8192, 8192, 15212, 7193},
                                             { 8192, 12969, 8192, 8192, 14316, 7760},
                                             { 8192, 16359, 8192, 8192, 16249, 8111}}};

//All the filters (how to call them)
const secondOrdfilters* filters[nbFilters] = {&sOFilters1,&sOFilters2,&sOFilters3,&sOFilters4,&sOFilters5};

#endif /* INCLUDES_COEFFSIIR_H_ */
