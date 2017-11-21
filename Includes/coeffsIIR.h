

#ifndef INCLUDES_COEFFSIIR_H_
#define INCLUDES_COEFFSIIR_H_
//**********ELLIPTIC FILTERS **********
//	Order = 4
//	Representation in 2Q13
#define nbFilters 2
#define nbSecondOrder 2
#define nbCoeffs 6

//Second Order Filters Structure
typedef struct {
	const int globalGain[nbSecondOrder];
	const int coeffsIIR[nbSecondOrder][nbCoeffs];} secondOrdfilters; 

//FILTER 1
const secondOrdfilters sOFilters1 =  {{147}, {{ 442, -434, 442, 16384, -30600, 15928},
{ 393753, -785998, 393753, 16384, -31737, 16083}}};

//FILTER 2
const secondOrdfilters sOFilters2 =  {{973}, {{ 442, -434, 442, 16384, -30600, 15928},
{ 393753, -785998, 393753, 16384, -31737, 16083}}};


//All the filters (how to call them)
const secondOrdfilters* filters[nbFilters] = {&sOFilters1,&sOFilters2}; 

#endif /* INCLUDES_COEFFSIIR_H_ */
