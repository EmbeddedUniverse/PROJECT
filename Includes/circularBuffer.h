#include <stdbool.h>


typedef struct{
    float* current;
    float* triggerPoints[4];
    int    size;
}circularQBuffer;

circularQBuffer initQBuffer(float *adress,int size);
bool quarterReached(circularQBuffer myBuffer);
void add2QBuffer(float value, circularQBuffer* buffer);
