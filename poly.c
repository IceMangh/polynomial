#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Polynomial* Poly_Create(int degree, const FieldInfo* type) {
    Polynomial* p = (Polynomial*)malloc(sizeof(Polynomial));
    p->degree = degree;
    p->type = type;
    p->coefficients = calloc(degree + 1, type->size);

    return p;
}


Polynomial* Poly_Enter(Polynomial* p) {
    if (!p) return NULL;

    for (int i = p->degree; i >= 0; i--) {
        p->type->scan(Poly_Get(p, i));
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
    if (a->type != b->type) {
        return NULL;
    }

    int maxDeg = (a->degree > b->degree) ? a->degree : b->degree;
    Polynomial* res = Poly_Create(maxDeg, a->type);

    char zeroBuf[32];
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
    if (a->type != b->type) {
        return NULL;
    }

    int newDeg = a->degree + b->degree;
    Polynomial* res = Poly_Create(newDeg, a->type);

    char tempMul[32];
    char tempSum[32];

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

    char temp[32];
    for (int i = p->degree - 1; i >= 0; i--) {
        p->type->mult(result, x, temp);
        p->type->add(temp, Poly_Get(p, i), result);
    }
}

Polynomial* Poly_Compose(const Polynomial* p, const Polynomial* q) {
    if (p->type != q->type) {
        return NULL;
    }

    Polynomial* acc = Poly_Create(0, p->type);
    Poly_Set(acc, 0, Poly_Get(p, p->degree));

    for (int i = p->degree - 1; i >= 0; i--) {
        Polynomial* multRes = Poly_Mult(acc, q);
        Poly_Free(acc);
        acc = multRes;

        Polynomial* constPoly = Poly_Create(0, p->type);
        Poly_Set(constPoly, 0, Poly_Get(p, i));

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

