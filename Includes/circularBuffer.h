#include <stdbool.h>


typedef struct{
    float* current;
    float* last;
    float* triggerPoints[4];
    int    size;
    bool quarterFlag;
}circularQBuffer;

typedef struct{
    float* current;
    float* last;
    int    size;
}circularBuffer;

circularQBuffer initQBuffer(float *adress, int size);
void add2QBuffer(float value, circularQBuffer* buffer);

circularBuffer initCBuffer(float *adress,int size);
void add2CBuffer(float value, circularQBuffer* buffer);
