#include "firFilter.h"

long long FIR_compute(short z[], short b[], int N)
{
    long long sum = 0;

    int i;
    for (i = 0; i < N; ++i)
        sum += z[i] * b[i];

    return sum;
}
