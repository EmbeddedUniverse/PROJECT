#include "circularBuffer.h"

float *add2CircularBuffer(float *adress, int sizeBuffer, float value);

void add2QBuffer(float value, circularQBuffer *buffer){
    buffer->current = add2CircularBuffer(buffer->current, buffer->size, value);
}

circularQBuffer initQBuffer(float *adress, int buffersize){

    circularQBuffer load;

    load.current = adress;
    load.size = buffersize;
    load.triggerPoints[0] = adress;
    load.triggerPoints[1] = adress +(buffersize/4);
    load.triggerPoints[2] = adress + buffersize/2;
    load.triggerPoints[3] = adress + buffersize - buffersize/4;

    return load;
}

bool quarterReached(circularQBuffer myBuffer){
    return myBuffer.current==myBuffer.triggerPoints[0] ||
           myBuffer.current==myBuffer.triggerPoints[1] ||
           myBuffer.current==myBuffer.triggerPoints[2] ||
           myBuffer.current==myBuffer.triggerPoints[3];
}
