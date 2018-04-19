#include <cstdio>
#include "sub_numbers.h"

int sub_numbers(int a, int b)
{
    printf("Called function from shared library\n");
    return a - b;
}
