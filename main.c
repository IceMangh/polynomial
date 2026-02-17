#include "poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Функция для ручного ввода полинома
Polynomial* input_poly(const FieldInfo* type) {
    int deg;
    printf("Введите степень многочлена: ");
    if (scanf("%d", &deg) != 1 || deg < 0) {
        printf("Некорректная степень.\n");
        return NULL;
    }

    Polynomial* p = Poly_Create(deg, type);
    printf("Введите коэффициенты (от a%d до a0 через пробел): ", deg);

    // Ввод зависит от типа данных
    Polynomial* P = Poly_Enter(p, type);
    return P;
}

// Запуск автоматических тестов
void run_tests() {

    Polynomial* p1 = Poly_Create(1, &INT_FIELD_INFO);
    int v = 1; Poly_Set(p1, 0, &v);
    v = 2; Poly_Set(p1, 1, &v);

    Polynomial* p2 = Poly_Create(1, &INT_FIELD_INFO);
    v = 5; Poly_Set(p2, 0, &v);
    v = 3; Poly_Set(p2, 1, &v);

    printf("A: "); Poly_Print(p1);
    printf("B: "); Poly_Print(p2);
    Polynomial* sum = Poly_Add(p1, p2);
    printf("A + B: "); Poly_Print(sum);


}

int main() {
    int choice;
    const FieldInfo* currentType = &INT_FIELD_INFO;

    printf("Выберите тип данных:\n1. Целые числа (Integer)\n2. Вещественные (Double)\n> ");
    if (scanf("%d", &choice) == 1 && choice == 2) {
        currentType = &DBL_FIELD_INFO;
    }

    Polynomial *pA = NULL, *pB = NULL, *pRes = NULL;

    while (1) {
        printf("\n=== МЕНЮ (%s) ===\n", currentType->typeName);
        printf("1. Ввести полином A\n");
        printf("2. Ввести полином B\n");
        printf("3. Сложить (A + B)\n");
        printf("4. Умножить (A * B)\n");
        printf("5. Вычислить A(x)\n");
        printf("6. Композиция A(B(x))\n");
        printf("7. Умножить A на скаляр\n");
        printf("8. Запустить тесты\n");
        printf("9. Вывести полиномы\n");
        printf("0. Выход\n");
        printf("> ");

        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                if (pA) Poly_Free(pA);
                pA = input_poly(currentType);
                if (pA) { printf("A = "); Poly_Print(pA); }
                break;
            case 2:
                if (pB) Poly_Free(pB);
                pB = input_poly(currentType);
                if (pB) { printf("B = "); Poly_Print(pB); }
                break;
            case 3:
                if (pA && pB) {
                    pRes = Poly_Add(pA, pB);
                    printf("Результат: "); Poly_Print(pRes);
                    Poly_Free(pRes);
                } else printf("Сначала введите полиномы A и B.\n");
                break;
            case 4:
                if (pA && pB) {
                    pRes = Poly_Mult(pA, pB);
                    printf("Результат: "); Poly_Print(pRes);
                    Poly_Free(pRes);
                } else printf("Сначала введите полиномы A и B.\n");
                break;
            case 5:
                if (pA) {
                    printf("Введите значение x: ");
                    if (currentType == &INT_FIELD_INFO) {
                        int x, r; scanf("%d", &x);
                        Poly_Eval(pA, &x, &r);
                        printf("A(%d) = %d\n", x, r);
                    } else {
                        double x, r; scanf("%lf", &x);
                        Poly_Eval(pA, &x, &r);
                        printf("A(%.2f) = %.2f\n", x, r);
                    }
                } else printf("Сначала введите полином A.\n");
                break;
            case 6:
                if (pA && pB) {
                    pRes = Poly_Compose(pA, pB);
                    printf("Результат A(B(x)): "); Poly_Print(pRes);
                    Poly_Free(pRes);
                } else printf("Сначала введите полиномы A и B.\n");
                break;
            case 7:
                if (pA) {
                    printf("Введите скаляр: ");
                    if (currentType == &INT_FIELD_INFO) {
                        int s; scanf("%d", &s);
                        pRes = Poly_MultScalar(pA, &s);
                    } else {
                        double s; scanf("%lf", &s);
                        pRes = Poly_MultScalar(pA, &s);
                    }
                    printf("Результат: "); Poly_Print(pRes);
                    Poly_Free(pRes);
                } else printf("Сначала введите полином A.\n");
                break;
            case 8:
                run_tests();
                break;
            case 9:
                Poly_Print(pA);
                Poly_Print(pB);
                break;
            case 0:
                goto cleanup;
            default:
                printf("Неверная команда.\n");
        }
    }

    cleanup:
    Poly_Free(pA);
    Poly_Free(pB);
    return 0;
}