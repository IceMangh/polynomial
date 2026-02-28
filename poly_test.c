#include "poly.h"
#include "scalars.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void test_integer(void) {
    printf("=== Тесты для Integer ===\n");
    // Create + Set/Get
    {
        Polynomial* p = Poly_Create(2, INT_FIELD_INFO());
        assert(p != NULL);
        int v = 5;  Poly_Set(p, 0, &v);
        v = -3; Poly_Set(p, 1, &v);
        v = 7;  Poly_Set(p, 2, &v);
        assert(*(int*)Poly_Get(p, 0) == 5);
        assert(*(int*)Poly_Get(p, 1) == -3);
        Poly_Free(p);
    }
    // Add
    {
        Polynomial* a = Poly_Create(1, INT_FIELD_INFO()); // 3x + 2
        int v = 2; Poly_Set(a, 0, &v);
        v = 3; Poly_Set(a, 1, &v);
        Polynomial* b = Poly_Create(1, INT_FIELD_INFO()); // x + 5
        v = 5; Poly_Set(b, 0, &v);
        v = 1; Poly_Set(b, 1, &v);
        Polynomial* sum = Poly_Add(a, b);
        assert(sum->degree == 1);
        assert(*(int*)Poly_Get(sum, 0) == 7);
        assert(*(int*)Poly_Get(sum, 1) == 4);
        Poly_Free(a); Poly_Free(b); Poly_Free(sum);
    }
    // Mult
    {
        Polynomial* a = Poly_Create(1, INT_FIELD_INFO()); // 3x + 2
        int v = 2; Poly_Set(a, 0, &v);
        v = 3; Poly_Set(a, 1, &v);
        Polynomial* b = Poly_Create(1, INT_FIELD_INFO()); // x + 5
        v = 5; Poly_Set(b, 0, &v);
        v = 1; Poly_Set(b, 1, &v);
        Polynomial* prod = Poly_Mult(a, b);
        assert(*(int*)Poly_Get(prod, 0) == 10);
        assert(*(int*)Poly_Get(prod, 1) == 17);
        assert(*(int*)Poly_Get(prod, 2) == 3);
        Poly_Free(a); Poly_Free(b); Poly_Free(prod);
    }
    // MultScalar
    {
        Polynomial* p = Poly_Create(1, INT_FIELD_INFO()); // 3x + 2
        int v = 2; Poly_Set(p, 0, &v);
        v = 3; Poly_Set(p, 1, &v);
        int s = 4;
        Polynomial* res = Poly_MultScalar(p, &s);
        assert(*(int*)Poly_Get(res, 0) == 8);
        assert(*(int*)Poly_Get(res, 1) == 12);
        Poly_Free(p); Poly_Free(res);
    }
    // Eval
    {
        Polynomial* p = Poly_Create(2, INT_FIELD_INFO()); // 2x² + 3x + 1
        int v = 1; Poly_Set(p, 0, &v);
        v = 3; Poly_Set(p, 1, &v);
        v = 2; Poly_Set(p, 2, &v);
        int x = 2, result;
        Poly_Eval(p, &x, &result);
        assert(result == 15);
        Poly_Free(p);
    }
    // Compose
    {
        Polynomial* p = Poly_Create(1, INT_FIELD_INFO()); // 2x + 1
        int v = 1; Poly_Set(p, 0, &v);
        v = 2; Poly_Set(p, 1, &v);
        Polynomial* q = Poly_Create(1, INT_FIELD_INFO()); // x + 3
        v = 3; Poly_Set(q, 0, &v);
        v = 1; Poly_Set(q, 1, &v);
        Polynomial* comp = Poly_Compose(p, q);
        assert(comp->degree == 1);
        assert(*(int*)Poly_Get(comp, 0) == 7);
        Poly_Free(p); Poly_Free(q); Poly_Free(comp);
    }
    // Derivative
    {
        Polynomial* p = Poly_Create(2, INT_FIELD_INFO()); // 4x² + 5x + 6
        int v = 6; Poly_Set(p, 0, &v);
        v = 5; Poly_Set(p, 1, &v);
        v = 4; Poly_Set(p, 2, &v);
        Polynomial* der = Poly_Derivative(p);
        assert(der->degree == 1);
        assert(*(int*)Poly_Get(der, 0) == 5);
        assert(*(int*)Poly_Get(der, 1) == 8);
        Poly_Free(p); Poly_Free(der);
    }
    printf("✓ Integer — все тесты пройдены!\n\n");
}

static void test_double(void) {
    printf("=== Тесты для Double ===\n");
    // Create + Set/Get + MultScalar
    {
        Polynomial* p = Poly_Create(2, DBL_FIELD_INFO());
        double v = 1.5; Poly_Set(p, 0, &v);
        v = -2.75; Poly_Set(p, 1, &v);
        v = 4.0; Poly_Set(p, 2, &v);
        double s = 2.0;
        Polynomial* res = Poly_MultScalar(p, &s);
        assert(fabs(*(double*)Poly_Get(res, 0) - 3.0) < 1e-9);
        Poly_Free(p); Poly_Free(res);
    }
    // Add
    {
        Polynomial* a = Poly_Create(1, DBL_FIELD_INFO()); // 2.5x + 1.2
        double v = 1.2; Poly_Set(a, 0, &v);
        v = 2.5; Poly_Set(a, 1, &v);
        Polynomial* b = Poly_Create(1, DBL_FIELD_INFO()); // 1.5x + 3.8
        v = 3.8; Poly_Set(b, 0, &v);
        v = 1.5; Poly_Set(b, 1, &v);
        Polynomial* sum = Poly_Add(a, b);
        assert(fabs(*(double*)Poly_Get(sum, 0) - 5.0) < 1e-9);
        assert(fabs(*(double*)Poly_Get(sum, 1) - 4.0) < 1e-9);
        Poly_Free(a); Poly_Free(b); Poly_Free(sum);
    }
    // Eval
    {
        Polynomial* p = Poly_Create(2, DBL_FIELD_INFO()); // 2x² + 3x + 1
        double v = 1.0; Poly_Set(p, 0, &v);
        v = 3.0; Poly_Set(p, 1, &v);
        v = 2.0; Poly_Set(p, 2, &v);
        double x = 2.0, result;
        Poly_Eval(p, &x, &result);
        assert(fabs(result - 15.0) < 1e-9);
        Poly_Free(p);
    }
    // Compose
    {
        Polynomial* p = Poly_Create(1, DBL_FIELD_INFO()); // 2x + 1
        double v = 1.0; Poly_Set(p, 0, &v);
        v = 2.0; Poly_Set(p, 1, &v);
        Polynomial* q = Poly_Create(1, DBL_FIELD_INFO()); // x + 3
        v = 3.0; Poly_Set(q, 0, &v);
        v = 1.0; Poly_Set(q, 1, &v);
        Polynomial* comp = Poly_Compose(p, q);
        assert(comp->degree == 1);
        assert(fabs(*(double*)Poly_Get(comp, 0) - 7.0) < 1e-9);
        Poly_Free(p); Poly_Free(q); Poly_Free(comp);
    }
    // Derivative
    {
        Polynomial* p = Poly_Create(2, DBL_FIELD_INFO()); // 3x² + 2x + 1
        double v = 1.0; Poly_Set(p, 0, &v);
        v = 2.0; Poly_Set(p, 1, &v);
        v = 3.0; Poly_Set(p, 2, &v);
        Polynomial* der = Poly_Derivative(p);
        assert(der->degree == 1);
        assert(fabs(*(double*)Poly_Get(der, 0) - 2.0) < 1e-9);
        assert(fabs(*(double*)Poly_Get(der, 1) - 6.0) < 1e-9);
        Poly_Free(p); Poly_Free(der);
    }
    printf("✓ Double — все тесты пройдены!\n\n");
}

void run_tests(void) {
    test_integer();
    test_double();
}