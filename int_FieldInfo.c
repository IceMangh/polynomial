#include "scalars.h"
#include <stdio.h>
#include <stdlib.h>

static void Int_Zero(void* d) {
    *(int*)d = 0;
}

static void Int_Add(const void* a, const void* b, void* r) {
    *(int*)r = *(const int*)a + *(const int*)b;
}

static void Int_Mult(const void* a, const void* b, void* r) {
    *(int*)r = *(const int*)a * *(const int*)b;
}

static void Int_From_Int(void* d, int v) {
    *(int*)d = v;
}

static void Int_Print(const void* a) {
    printf("%d", *(const int*)a);
}

static void Int_Scan(void* d) {
    scanf("%d", (int*)d);
}

const FieldInfo* INT_FIELD_INFO(void) {
    static const FieldInfo info = {
            "Integer",
            sizeof(int),
            Int_Zero,
            Int_Add,
            Int_Mult,
            Int_Print,
            Int_Scan,
            Int_From_Int
    };
    return &info;
}