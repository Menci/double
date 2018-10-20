#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "double.h"
#include "double_input.h"
#include "double_output.h"
#include "double_helpers.h"
#include "eval.h"
#include "int128.h"

uint32_t rand32() {
	return ((uint32_t)rand() << 16 >> 16) | ((uint32_t)rand() << 16);
}

uint64_t rand64() {
    return (((uint64_t)rand32()) << 32) | ((uint64_t)rand32());
}

double rand_double() {
    // The first method can generate more edge case while the second
    // method can prevent one number is less/greater than too many times
    // of another.
    if (rand() & 1) {
        union {
            unsigned long long uint64_t_value;
            double double_value;
        } u;
        u.uint64_t_value = rand64();
        return u.double_value;
    } else {
        double a = (double)rand64() * (double)(1ull << 63) + rand64(),
               b = (double)rand64() * (double)(1ull << 63) + rand64();
        int sign = (rand() & 1) ? 1 : -1;
        return a / b * sign;
    }
}

/*
void check_uint128() {
    srand(1);
    for (int i = 1; ; i++) {
        unsigned __int128 a = rand128();
        unsigned __int128 b = rand128();
        unsigned __int128 c = a + b;

        _uint128_t ta = *(_uint128_t *)&a;
        _uint128_t tb = *(_uint128_t *)&b;
        _uint128_t tc = _uint128_add(ta, tb);

        unsigned __int128 c2 = *(unsigned __int128 *)&tc;

        if (c != c2) {
            INT3;
            printf("Wrong Answer on Test #%d.\n", i);
            break;
        }

        printf("Accepted on Test #%d.\n", i);
    }
}
*/

void check() {
    int denormal_count = 0;
    for (int i = 1; ; i++) {
        double a = rand_double();
        double b = rand_double();
        double c = a / b;

        double_t ta = fp_reinterpret_from_double(a);
        double_t tb = fp_reinterpret_from_double(b);
        double_t tc = fp_div(ta, tb);

        if (fp_isdenormal(ta)) denormal_count++;
        if (fp_isdenormal(tb)) denormal_count++;

        double c2 = fp_reinterpret_to_double(tc);

        int isnan(double);
        if (c != c2 && !(fp_isnan(tc) && isnan(c))) {
            printf("Wrong Answer on Test #%d.\n", i);
            fp_inspect(ta);
            fp_inspect(tb);
            fp_inspect(fp_reinterpret_from_double(c));
            fp_inspect(tc);
            break;
        }

        if (i % 100000 == 0) printf("Accepted on Test #%d, denormal_count = %d.\n", i, denormal_count);
    }
}

void debug() {
    double dx, dy;
    // scanf("%lf %lf", &dx, &dy);
    *(unsigned long long *)&dx = 0b0100000000000000111001010101111001110010111011110010000100011001;
    *(unsigned long long *)&dy = 0b0011111111110111011111100110101111000111010011010011010011011001;
    
    double_t x = fp_reinterpret_from_double(dx), y = fp_reinterpret_from_double(dy);
    fp_inspect(x);
    fp_inspect(y);

    fp_inspect(fp_reinterpret_from_double(dx + dy));

    double_t z = fp_add(x, y);
    fp_inspect(z);

    puts(fp_reinterpret_to_double(z) == dx + dy ? "Result matched." : "Result NOT matched.");
}

void check_io() {
    char s[100];
    while (~scanf("%s", s)) {
        double_t result_atof = fp_reinterpret_from_double(atof(s));
        double_t result_input;
        int len;
        bool success = fp_input(s, &result_input, &len);

        fp_inspect(result_atof);
        fp_inspect(result_input);
        puts(success ? "Parse success." : "Prase FAILED.");
        printf("Prased length = %d\n", len);
        puts(fp_reinterpret_to_double(result_atof) == fp_reinterpret_to_double(result_input) ? "Result matched." : "Result NOT matched.");

        char buffer[10000];
        fp_output(result_input, 5, buffer);
        printf("buffer = %s\n", buffer);
    }
}

void check_eval() {
    char s[1000000];
    while (~scanf("%s", s)) {
        double_t result = eval(s);
        char buffer[1000000];
        fp_inspect(result);
        fp_output(result, 10, buffer);
        printf("%s\n", buffer);
    }
}

int main() {
    // printf("%d\n", (int)sizeof(double_t));

    // double_t x;
    // x.sign = 1;
    // x.exponent = 2;
    // x.fraction = 3;

    // unsigned long long y = *((unsigned long long *)&x);
    // for (int i = 63; i >= 0; i--) printf("%d%c", !!(y & (1ull << i)), "\0\n"[!i]);

    // debug();

    // check();
    // check_uint128();

    // double log2(double);
    // double t = log2(10);
    // double_t tt = *(double_t *)&t;
    // fp_inspect(tt);

    // unsigned long long x;
    // while (~scanf("%lld", &x)) fp_inspect(_fp_from_uint128(x));

    check_io();

    // check_eval();
}

#endif
