#include "poly_test.h"
#include "poly.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

void run_tests(void) {
    printf("=== ЗАПУСК ТЕСТОВ (assert) ===\n\n");

    {
        Polynomial* p1 = Poly_Create(1, &INT_FIELD_INFO); // 3x + 2
        int v = 2; Poly_Set(p1, 0, &v);
        v = 3;     Poly_Set(p1, 1, &v);

        Polynomial* p2 = Poly_Create(1, &INT_FIELD_INFO); // x + 5
        v = 5; Poly_Set(p2, 0, &v);
        v = 1; Poly_Set(p2, 1, &v);

        Polynomial* sum = Poly_Add(p1, p2);

        assert(sum->degree == 1);
        assert(*(int*)Poly_Get(sum, 0) == 7);
        assert(*(int*)Poly_Get(sum, 1) == 4);

        Poly_Free(p1); Poly_Free(p2); Poly_Free(sum);
        printf("✓ Add integers — OK\n");
    }

    {
        Polynomial* p1 = Poly_Create(2, &INT_FIELD_INFO); // -4x^2 + 3x + 2
        int v = 2;
        Poly_Set(p1, 0, &v);
        v = 3;
        Poly_Set(p1, 1, &v);
        v = -4;
        Poly_Set(p1, 2, &v);

        Polynomial* p2 = Poly_Create(1, &INT_FIELD_INFO); // x + 5
        v = 5; Poly_Set(p2, 0, &v);
        v = 1;     Poly_Set(p2, 1, &v);

        Polynomial* prod = Poly_Mult(p1, p2);

        assert(prod->degree == 3);
        assert(*(int*)Poly_Get(prod, 0) == 10);
        assert(*(int*)Poly_Get(prod, 1) == 17);
        assert(*(int*)Poly_Get(prod, 2) == -17);
        assert(*(int*)Poly_Get(prod, 3) == -4);

        Poly_Free(p1); Poly_Free(p2); Poly_Free(prod);
        printf("✓ Mult integers — OK\n");
    }

    {
        Polynomial* p = Poly_Create(2, &DBL_FIELD_INFO); // 2x² + 3x + 1
        double v = 1.0; Poly_Set(p, 0, &v);
        v = 3.0;        Poly_Set(p, 1, &v);
        v = 2.0;        Poly_Set(p, 2, &v);

        double x = 2.0, result;
        Poly_Eval(p, &x, &result);
        assert(fabs(result - 15.0) < 1e-9);

        Poly_Free(p);
        printf("✓ Eval double — OK\n");
    }

    {
        Polynomial* p = Poly_Create(1, &INT_FIELD_INFO); // 2x + 1
        int v = 1; Poly_Set(p, 0, &v);
        v = 2;     Poly_Set(p, 1, &v);

        Polynomial* q = Poly_Create(1, &INT_FIELD_INFO); // x + 3
        v = 3; Poly_Set(q, 0, &v);
        v = 1; Poly_Set(q, 1, &v);

        Polynomial* comp = Poly_Compose(p, q);

        assert(comp->degree == 1);
        assert(*(int*)Poly_Get(comp, 0) == 7);
        assert(*(int*)Poly_Get(comp, 1) == 2);

        Poly_Free(p); Poly_Free(q); Poly_Free(comp);
        printf("✓ Compose — OK\n");
    }

    {
        Polynomial* p = Poly_Create(1, &DBL_FIELD_INFO); // 5x + 4
        double v = 4.0; Poly_Set(p, 0, &v);
        v = 5.0;        Poly_Set(p, 1, &v);

        double scalar = 2.5;
        Polynomial* res = Poly_MultScalar(p, &scalar);

        assert(fabs(*(double*)Poly_Get(res, 0) - 10.0) < 1e-9);
        assert(fabs(*(double*)Poly_Get(res, 1) - 12.5) < 1e-9);

        Poly_Free(p); Poly_Free(res);
        printf("✓ MultScalar double — OK\n");
    }

    printf("\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! ===\n");
}