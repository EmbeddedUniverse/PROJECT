#include <stdbool.h>


typedef struct{
    float* current;
    float* last;
    float* triggerPoints[4];
    int    size;
    bool quarterFlag;
}circularQBuffer;

circularQBuffer initQBuffer(float *adress,int size);
bool quarterReached(circularQBuffer myBuffer);
void add2QBuffer(float value, circularQBuffer* buffer);
