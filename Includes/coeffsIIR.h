

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
const secondOrdfilters sOFilters1 =  {{32}, {{ 8192, -11738, 8192, 8192, -13762, 6842},
{ 8192, -16323, 8192, 8192, -15579, 7607},
{ 8192, -15091, 8192, 8192, -15406, 8045},
{ 8192, -16139, 8192, 8192, -15904, 8092},
{ 8192, -15307, 8192, 8192, -15363, 8172},
{ 8192, -16089, 8192, 8192, -16054, 8180}}};

//FILTER 2
const secondOrdfilters sOFilters2 =  {{77}, {{ 8192, -16323, 8192, 8192, -15579, 7607},
{ 8192, -16139, 8192, 8192, -15904, 8092},
{ 8192, -16089, 8192, 8192, -16054, 8180},
{ 8192, 15334, 8192, 8192, 8951, 5795},
{ 8192, 12533, 8192, 8192, 10505, 7844},
{ 8192, 11842, 8192, 8192, 11509, 8150}}};


//All the filters (how to call them)
const secondOrdfilters* filters[nbFilters] = {&sOFilters1,&sOFilters2}; 

#endif /* INCLUDES_COEFFSIIR_H_ */
