#include <cstdio>
#include "add_numbers.h"

int add_numbers(int a, int b)
{
    printf("called function from static library\n");
    return a + b;
}
