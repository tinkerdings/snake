#include "util.h"

int iRandRange(int low, int high)
{
    return (rand() % (++high-low)) + low;
}
