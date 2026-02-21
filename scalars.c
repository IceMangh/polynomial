#include "scalars.h"
#include <stdio.h>



static void int_zero(void* d) {
    *(int*)d = 0;
}

static void int_add(const void* a, const void* b, void* r) {
    *(int*)r = *(const int*)a + *(const int*)b;
}

static void int_mult(const void* a, const void* b, void* r) {
    *(int*)r = *(const int*)a * *(const int*)b;
}

static void int_print(const void* a) {
    printf("%d", *(const int*)a);
}

static void int_scan(void* d) {
    scanf("%d", (int*)d);
}

const FieldInfo INT_FIELD_INFO = {
        "Integer",
        sizeof(int),
        int_zero,
        int_add,
        int_mult,
        int_print,
        int_scan
};





static void dbl_zero(void* d) {
    *(double*)d = 0.0;
}

static void dbl_add(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a + *(const double*)b;
}

static void dbl_mult(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a * *(const double*)b;
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
        dbl_scan
};