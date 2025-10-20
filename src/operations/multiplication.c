#include "operations/multiplication.h"
#include "helpers.h"

int operations_multiply (int a, int b) {
    int aa = nonstaticfun_plus_three(a);
    return (aa - 3) * b;
}
