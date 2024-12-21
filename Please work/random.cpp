#include "main.h"
#include "random.h"
uint16 gRngValue;

uint32 Random(void)
{
    gRngValue = 1103515245 * gRngValue + 24691;
    return gRngValue >> 16;
}

void SeedRng(uint16 seed)
{
    gRngValue = seed;
}