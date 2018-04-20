#include <cstdio>
#include "mul_numbers.h"

extern "C"
int mul_numbers(int a, int b)
{
    printf("Called function from loadable library\n");
    return a * b;
}
