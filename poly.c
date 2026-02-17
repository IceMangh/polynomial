#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Polynomial* Poly_Create(int degree, const FieldInfo* type) {
    Polynomial* p = (Polynomial*)malloc(sizeof(Polynomial));
    p->degree = degree;
    p->type = type;
    p->coefficients = calloc(degree + 1, type->size);

//    for (int i = 0; i <= degree; i++) {
//        type->zero((char*)p->coefficients + i * type->size);
//    }
    return p;
}


Polynomial* Poly_Enter(Polynomial* p, const FieldInfo* type){
    if (strcmp(type->typeName, "Integer") == 0) {
        for (int i = p->degree; i >= 0; i--) {
            int val;
            scanf("%d", &val);
            Poly_Set(p, i, &val);
        }
    } else {
        for (int i = p->degree; i >= 0; i--) {
            double val;
            scanf("%lf", &val);
            Poly_Set(p, i, &val);
        }
    }
    return p;
}

void Poly_Free(Polynomial* p) {
    if (p) {
        free(p->coefficients);
        free(p);
    }
}

void* Poly_Get(const Polynomial* p, int index) {
    if (index < 0 || index > p->degree) return NULL;
    return (char*)p->coefficients + index * p->type->size;
}

void Poly_Set(Polynomial* p, int index, const void* value) {
    if (index >= 0 && index <= p->degree) {
        memcpy((char*)p->coefficients + index * p->type->size, value, p->type->size);
    }
}

Polynomial* Poly_Add(const Polynomial* a, const Polynomial* b) {
    if (a->type != b->type) return NULL;

    int maxDeg = (a->degree > b->degree) ? a->degree : b->degree;
    Polynomial* res = Poly_Create(maxDeg, a->type);

    // Временный буфер для нуля
    char zeroBuf[64];
    a->type->zero(zeroBuf);

    for (int i = 0; i <= maxDeg; i++) {
        void* r_val = Poly_Get(res, i);
        void* a_val = (i <= a->degree) ? Poly_Get(a, i) : zeroBuf;
        void* b_val = (i <= b->degree) ? Poly_Get(b, i) : zeroBuf;

        a->type->add(a_val, b_val, r_val);
    }
    return res;
}

Polynomial* Poly_Mult(const Polynomial* a, const Polynomial* b) {
    if (a->type != b->type) return NULL;

    int newDeg = a->degree + b->degree;
    Polynomial* res = Poly_Create(newDeg, a->type);

    char tempMul[64];
    char tempSum[64];

    for (int i = 0; i <= a->degree; i++) {
        for (int j = 0; j <= b->degree; j++) {
            void* resPtr = Poly_Get(res, i + j);
            void* aPtr = Poly_Get(a, i);
            void* bPtr = Poly_Get(b, j);

            a->type->mult(aPtr, bPtr, tempMul);
            a->type->add(resPtr, tempMul, tempSum);
            memcpy(resPtr, tempSum, a->type->size);
        }
    }
    return res;
}

Polynomial* Poly_MultScalar(const Polynomial* a, const void* scalar) {
    Polynomial* res = Poly_Create(a->degree, a->type);
    for (int i = 0; i <= a->degree; i++) {
        a->type->mult(Poly_Get(a, i), scalar, Poly_Get(res, i));
    }
    return res;
}

void Poly_Eval(const Polynomial* p, const void* x, void* result) {
    memcpy(result, Poly_Get(p, p->degree), p->type->size);

    char temp[64];
    for (int i = p->degree - 1; i >= 0; i--) {
        p->type->mult(result, x, temp);
        p->type->add(temp, Poly_Get(p, i), result);
    }
}

Polynomial* Poly_Compose(const Polynomial* p, const Polynomial* q) {
    if (p->type != q->type) return NULL;

    // acc = a_n (старший коэффициент P как константа)
    Polynomial* acc = Poly_Create(0, p->type);
    Poly_Set(acc, 0, Poly_Get(p, p->degree));

    for (int i = p->degree - 1; i >= 0; i--) {
        // acc = acc * q
        Polynomial* multRes = Poly_Mult(acc, q);
        Poly_Free(acc);
        acc = multRes;

        // Создаем константный полином a[i]
        Polynomial* constPoly = Poly_Create(0, p->type);
        Poly_Set(constPoly, 0, Poly_Get(p, i));

        // acc = acc + a[i]
        Polynomial* addRes = Poly_Add(acc, constPoly);

        Poly_Free(acc);
        Poly_Free(constPoly);
        acc = addRes;
    }
    return acc;
}

void Poly_Print(const Polynomial* p) {
    printf("Degree %d: [ ", p->degree);
    for (int i = p->degree; i >= 0; i--) {
        p->type->print(Poly_Get(p, i));
        if (i > 0) printf(" x^%d + ", i);
    }
    printf(" ]\n");
}

// =========================================================
// РЕАЛИЗАЦИЯ ТИПОВ (INT, DOUBLE)
// =========================================================

// --- Integer ---
//static void int_zero(void* d) { *(int*)d = 0; }
//static void int_add(const void* a, const void* b, void* r) { *(int*)r = *(const int*)a + *(const int*)b; }
//static void int_mult(const void* a, const void* b, void* r) { *(int*)r = *(const int*)a * *(const int*)b; }
//static void int_print(const void* a) { printf("%d", *(const int*)a); }
//
//const FieldInfo INT_FIELD_INFO = {
//        "Integer", sizeof(int), int_zero, int_add, int_mult, int_print
//};
//
//// --- Double ---
//static void dbl_zero(void* d) { *(double*)d = 0.0; }
//static void dbl_add(const void* a, const void* b, void* r) { *(double*)r = *(const double*)a + *(const double*)b; }
//static void dbl_mult(const void* a, const void* b, void* r) { *(double*)r = *(const double*)a * *(const double*)b; }
//static void dbl_print(const void* a) { printf("%.2f", *(const double*)a); }
//
//const FieldInfo DBL_FIELD_INFO = {
//        "Double", sizeof(double), dbl_zero, dbl_add, dbl_mult, dbl_print
//};