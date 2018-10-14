#ifdef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "double.h"
#include "int128.h"

uint64_t rand64() {
    return (((uint64_t)rand()) << 32) | ((uint64_t)rand());
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
        double a = rand64() * (1ull << 63) + rand64(),
               b = rand64() * (1ull << 63) + rand64();
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
        double c = a + b;

        double_t ta = fp_reinterpret_from_double(a);
        double_t tb = fp_reinterpret_from_double(b);
        double_t tc = fp_add(ta, tb);

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

        printf("Accepted on Test #%d, denormal_count = %d.\n", i, denormal_count);
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

int main() {
    // printf("%d\n", (int)sizeof(double_t));

    // double_t x;
    // x.sign = 1;
    // x.exponent = 2;
    // x.fraction = 3;

    // unsigned long long y = *((unsigned long long *)&x);
    // for (int i = 63; i >= 0; i--) printf("%d%c", !!(y & (1ull << i)), "\0\n"[!i]);

    debug();

    // check();
    // check_uint128();
}

#endif
