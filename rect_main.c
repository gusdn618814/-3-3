#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_math.h"

/* 임시: 링크 문제 우회용 구현 */
static long long gcd_local(long long a, long long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static void reduce_local(Fraction* f)
{
    long long g;

    if (f->den < 0) {
        f->den = -f->den;
        f->num = -f->num;
    }

    if (f->num == 0) {
        f->den = 1;
        return;
    }

    g = gcd_local(f->num, f->den);
    f->num /= g;
    f->den /= g;
}

bool frac_parse(const char* src, Fraction* out)
{
    char buf[100];
    char* slash;

    if (src == NULL || out == NULL) return false;

    slash = strchr(src, '/');
    if (slash == NULL) return false;

    if (strlen(src) >= sizeof(buf)) return false;
    strcpy(buf, src);

    slash = strchr(buf, '/');
    if (slash == NULL) return false;

    *slash = '\0';
    slash++;

    out->num = atoll(buf);
    out->den = atoll(slash);

    if (out->den == 0) return false;

    reduce_local(out);
    return true;
}

Fraction frac_add(Fraction a, Fraction b)
{
    Fraction r;
    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;
    reduce_local(&r);
    return r;
}

Fraction frac_mul(Fraction a, Fraction b)
{
    Fraction r;
    r.num = a.num * b.num;
    r.den = a.den * b.den;
    reduce_local(&r);
    return r;
}

int main(int argc, char** argv)
{
    Fraction w, h;
    Fraction sum;
    Fraction two;
    Fraction perimeter;
    Fraction area;

    if (argc != 3) {
        printf("Usage: Rectangle width height\n");
        system("pause");
        return 1;
    }

    if (!frac_parse(argv[1], &w) || !frac_parse(argv[2], &h)) {
        printf("Input error\n");
        system("pause");
        return 1;
    }

    sum = frac_add(w, h);

    two.num = 2;
    two.den = 1;

    perimeter = frac_mul(two, sum);
    area = frac_mul(w, h);

    printf("Perimeter: %lld/%lld, Area: %lld/%lld\n",
        perimeter.num, perimeter.den, area.num, area.den);

    system("pause");
    return 0;
}
