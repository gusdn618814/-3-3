#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "my_math.h"

/* ---------- GCD ---------- */
long long gcd_ll(long long a, long long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ---------- LCM (overflow-safe) ---------- */
bool lcm_ll(long long a, long long b, long long* result)
{
    if (a == 0 || b == 0) {
        *result = 0;
        return true;
    }

    long long g = gcd_ll(a, b);
    long long tmp = a / g;

    if (tmp > LLONG_MAX / b) return false;

    *result = tmp * b;
    return true;
}

/* ---------- Fraction ---------- */
static bool is_digits(const char* s)
{
    if (*s == '\0') return false;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

/* 정수형 문자열 → Fraction (num/1) */
static bool parse_int(const char* s, Fraction* out)
{
    char* end;
    long long v = strtoll(s, &end, 10);

    if (*end != '\0') return false;

    out->num = v;
    out->den = 1;
    return true;
}

/* a/b 형태 파싱 */
static bool parse_frac(const char* s, Fraction* out)
{
    char buf[128];
    char* slash;

    if (strlen(s) >= sizeof(buf)) return false;

    strcpy(buf, s);

    slash = strchr(buf, '/');
    if (!slash) return false;

    *slash = '\0';

    if (!is_digits(buf) || !is_digits(slash + 1)) return false;

    out->num = atoll(buf);
    out->den = atoll(slash + 1);

    if (out->den == 0) return false;

    return true;
}

/* 소수 → Fraction (예: 0.25 → 1/4) */
static bool parse_decimal(const char* s, Fraction* out)
{
    const char* dot;
    size_t digits = 0;
    long long den = 1;
    long long num;
    double d;

    dot = strchr(s, '.');
    if (!dot) return false;

    d = strtod(s, NULL);
    if (d < 0) return false;

    digits = strlen(dot + 1);

    for (size_t i = 0; i < digits; ++i) {
        den *= 10;
    }

    num = (long long)round(d * den);

    out->num = num;
    out->den = den;
    return true;
}

/* 외부 호출 API */
bool frac_parse(const char* src, Fraction* out)
{
    if (!src || !out) return false;

    if (parse_int(src, out)) {
        frac_reduce(out);
        return true;
    }

    if (parse_frac(src, out)) {
        frac_reduce(out);
        return true;
    }

    if (parse_decimal(src, out)) {
        frac_reduce(out);
        return true;
    }

    return false;
}

/* 약분 */
void frac_reduce(Fraction* f)
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

    g = gcd_ll(f->num, f->den);
    f->num /= g;
    f->den /= g;
}

/* 덧셈 */
Fraction frac_add(Fraction a, Fraction b)
{
    Fraction r;
    r.num = a.num * b.den + b.num * a.den;
    r.den = a.den * b.den;
    frac_reduce(&r);
    return r;
}

/* 곱셈 */
Fraction frac_mul(Fraction a, Fraction b)
{
    Fraction r;
    r.num = a.num * b.num;
    r.den = a.den * b.den;
    frac_reduce(&r);
    return r;
}
