#include <assert.h>
#include "double.h"

double benchmark_calc(double a, double b, char op) {
    if (op == '+')
        return fp_reinterpret_to_double(fp_add(fp_reinterpret_from_double(a), fp_reinterpret_from_double(b)));
    if (op == '-')
        return fp_reinterpret_to_double(fp_sub(fp_reinterpret_from_double(a), fp_reinterpret_from_double(b)));
    if (op == '*')
        return fp_reinterpret_to_double(fp_mul(fp_reinterpret_from_double(a), fp_reinterpret_from_double(b)));
    if (op == '/')
        return fp_reinterpret_to_double(fp_div(fp_reinterpret_from_double(a), fp_reinterpret_from_double(b)));
    assert(false);
}
