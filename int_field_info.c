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

static void int_from_int(void* d, int v) {
    *(int*)d = v;
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
        int_scan,
        int_from_int
};

