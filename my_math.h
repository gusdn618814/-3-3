#ifndef MY_MATH_H
#define MY_MATH_H

#include <stdbool.h>
#include <limits.h>

long long gcd_ll(long long a, long long b);
bool lcm_ll(long long a, long long b, long long* result);

typedef struct {
    long long num;
    long long den;
} Fraction;

bool frac_parse(const char* src, Fraction* out);
void frac_reduce(Fraction* f);
Fraction frac_add(Fraction a, Fraction b);
Fraction frac_mul(Fraction a, Fraction b);

#endif
