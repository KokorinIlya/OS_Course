//
// Created by ilya on 20.04.18.
//
#include <cstdio>
#include "print_mul.h"
#include "mul_numbers.h"

extern "C"
void print_mul(int a, int b)
{
    printf("%d\n", mul_numbers(a, b));
}
