

#ifndef INCLUDES_COEFFSIIR_H_
#define INCLUDES_COEFFSIIR_H_
//**********ELLIPTIC FILTERS **********
//	Order = 12
//	Representation in 2Q13
#define nbFilters 2
#define nbSecondOrder 6
#define nbCoeffs 6

//Second Order Filters Structure
typedef struct {
	const short globalGain[nbSecondOrder];
	const short coeffsIIR[nbSecondOrder][nbCoeffs];} secondOrdfilters; 

//FILTER 1
const secondOrdfilters sOFilters1 =  {{45}, {{ 971, -1710, 971, 8192, -15498, 8041},
{ 3198, -6334, 3198, 8192, -15839, 8076},
{ 2575, -4806, 2575, 8192, -15384, 8150},
{ 2818, -5536, 2818, 8192, -16028, 8167},
{ 5248, -9835, 5248, 8192, -15369, 8186},
{ 4694019, -9210972, 4694019, 8192, -16065, 8189}}};

//FILTER 2
const secondOrdfilters sOFilters2 =  {{691}, {{ 3198, -6334, 3198, 8192, -15839, 8076},
{ 2818, -5536, 2818, 8192, -16028, 8167},
{ 4694019, -9210972, 4694019, 8192, -16065, 8189},
{ 2188, 3823, 2188, 8192, 8993, 7588},
{ 2082, 3055, 2082, 8192, 11170, 8062},
{ 342738, 489265, 342738, 8192, 11554, 8175}}};


//All the filters (how to call them)
const secondOrdfilters* filters[nbFilters] = {&sOFilters1,&sOFilters2}; 

#endif /* INCLUDES_COEFFSIIR_H_ */
