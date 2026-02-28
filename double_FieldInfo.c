#include "scalars.h"
#include <stdio.h>

static void Dbl_Zero(void* d) {
    *(double*)d = 0.0;
}

static void Dbl_Add(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a + *(const double*)b;
}

static void Dbl_Mult(const void* a, const void* b, void* r) {
    *(double*)r = *(const double*)a * *(const double*)b;
}

static void Dbl_From_Int(void* d, int v) {
    *(double*)d = (double)v;
}

static void Dbl_Print(const void* a) {
    printf("%.2f", *(const double*)a);
}

static void Dbl_Scan(void* d) {
    scanf("%lf", (double*)d);
}

const FieldInfo* DBL_FIELD_INFO(void) {
    static const FieldInfo info = {
            "Double",
            sizeof(double),
            Dbl_Zero,
            Dbl_Add,
            Dbl_Mult,
            Dbl_Print,
            Dbl_Scan,
            Dbl_From_Int
    };
    return &info;
}