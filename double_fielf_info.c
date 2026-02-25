#include "scalars.h"
#include <stdio.h>



static void dbl_zero(void* d) {
    *(double*)d = 0.0;
}

static void dbl_add(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a + *(const double*)b;
}

static void dbl_mult(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a * *(const double*)b;
}

static void dbl_from_int(void* d, int v) {
    *(double*)d = (double)v;
}

static void dbl_print(const void* a) {
    printf("%.2f", *(const double*)a);
}

static void dbl_scan(void* d) {
    scanf("%lf", (double*)d);
}




const FieldInfo DBL_FIELD_INFO = {
        "Double",
        sizeof(double),
        dbl_zero,
        dbl_add,
        dbl_mult,
        dbl_print,
        dbl_scan,
        dbl_from_int
};