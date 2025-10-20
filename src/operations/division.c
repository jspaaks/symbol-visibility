#include "operations/division.h"
#include "helpers.h"

static int staticfun_plus_five (int i);

int operations_divide (int a, int b) {
    int aa = staticfun_plus_five(a);
    int bb = nonstaticfun_plus_three(b);
    return (aa - 5) / (bb - 3);
}

static int staticfun_plus_five (int i) {
    return i + 5;
}
