#include "Poly.h"
#include "Poly_Test.h"
#include "Scalars.h"
#include <stdio.h>

static void Clear_Input_Buffer(void) {
    int Character = 0;
    do {
        Character = getchar();
    } while (Character != '\n' && Character != EOF);
}

static void Print_Last_Error(void) {
    const char* Error_Message = Poly_Get_Last_Error();
    if (Error_Message != NULL) {
        fprintf(stderr, "Ошибка: %s\n", Error_Message);
    }
}

static Polynomial* Input_Polynomial(const Field_Info* Type) {
    int Degree = 0;
    printf("Введите степень многочлена: ");
    if (scanf("%d", &Degree) != 1 || Degree < 0) {
        Clear_Input_Buffer();
        printf("Некорректная степень.\n");
        return NULL;
    }

    Polynomial* Polynomial_Value = Poly_Create(Degree, Type);
    if (Polynomial_Value == NULL) {
        Print_Last_Error();
        return NULL;
    }

    printf("Введите коэффициенты (от a%d до a0 через пробел): ", Degree);
    if (Poly_Input(Polynomial_Value) != Poly_Status_Ok) {
        Clear_Input_Buffer();
        Print_Last_Error();
        Poly_Free(Polynomial_Value);
        return NULL;
    }

    return Polynomial_Value;
}

int main() {
    int Choice = 0;
    const Field_Info* Current_Type = Int_Field_Info();

    printf("Выберите тип данных:\n1. Целые числа (Integer)\n2. Вещественные (Double)\n> ");
    if (scanf("%d", &Choice) == 1) {
        if (Choice == 2) {
            Current_Type = Double_Field_Info();
        }
    } else {
        Clear_Input_Buffer();
        printf("Некорректный выбор, используется Integer.\n");
    }

    Polynomial* Polynomial_A = NULL;
    Polynomial* Polynomial_B = NULL;
    Polynomial* Polynomial_Result = NULL;
    int Continue_Running = 1;
    while (Continue_Running) {
        printf("\n=== МЕНЮ (%s) ===\n", Current_Type->Type_Name);
        printf("1. Ввести полином A\n");
        printf("2. Ввести полином B\n");
        printf("3. Сложить (A + B)\n");
        printf("4. Умножить (A * B)\n");
        printf("5. Вычислить A(x)\n");
        printf("6. Композиция A(B(x))\n");
        printf("7. Умножить A на скаляр\n");
        printf("8. Запустить тесты\n");
        printf("9. Вывести полиномы\n");
        printf("10. Взять производную A'\n");
        printf("0. Выход\n");
        printf("> ");

        if (scanf("%d", &Choice) != 1) {
            Clear_Input_Buffer();
            printf("Неверная команда.\n");
            continue;
        }

        switch (Choice) {
            case 1:
                Poly_Free(Polynomial_A);
                Polynomial_A = Input_Polynomial(Current_Type);
                if (Polynomial_A != NULL) {
                    printf("A = ");
                    Poly_Print(Polynomial_A);
                }
                break;
            case 2:
                Poly_Free(Polynomial_B);
                Polynomial_B = Input_Polynomial(Current_Type);
                if (Polynomial_B != NULL) {
                    printf("B = ");
                    Poly_Print(Polynomial_B);
                }
                break;
            case 3:
                if (Polynomial_A != NULL && Polynomial_B != NULL) {
                    Polynomial_Result = Poly_Add(Polynomial_A, Polynomial_B);
                    if (Polynomial_Result == NULL) {
                        Print_Last_Error();
                    } else {
                        printf("Результат: ");
                        Poly_Print(Polynomial_Result);
                        Poly_Free(Polynomial_Result);
                    }
                } else {
                    printf("Сначала введите полиномы A и B.\n");
                }
                break;
            case 4:
                if (Polynomial_A != NULL && Polynomial_B != NULL) {
                    Polynomial_Result = Poly_Multiply(Polynomial_A, Polynomial_B);
                    if (Polynomial_Result == NULL) {
                        Print_Last_Error();
                    } else {
                        printf("Результат: ");
                        Poly_Print(Polynomial_Result);
                        Poly_Free(Polynomial_Result);
                    }
                } else {
                    printf("Сначала введите полиномы A и B.\n");
                }
                break;
            case 5:
                if (Polynomial_A != NULL) {
                    printf("Введите значение x: ");
                    if (Current_Type == Int_Field_Info()) {
                        int X_Value = 0;
                        int Result = 0;
                        if (scanf("%d", &X_Value) != 1) {
                            Clear_Input_Buffer();
                            printf("Некорректное значение x.\n");
                            break;
                        }
                        if (Poly_Evaluate(Polynomial_A, &X_Value, &Result) != Poly_Status_Ok) {
                            Print_Last_Error();
                            break;
                        }
                        printf("A(%d) = %d\n", X_Value, Result);
                    } else {
                        double X_Value = 0.0;
                        double Result = 0.0;
                        if (scanf("%lf", &X_Value) != 1) {
                            Clear_Input_Buffer();
                            printf("Некорректное значение x.\n");
                            break;
                        }
                        if (Poly_Evaluate(Polynomial_A, &X_Value, &Result) != Poly_Status_Ok) {
                            Print_Last_Error();
                            break;
                        }
                        printf("A(%.2f) = %.2f\n", X_Value, Result);
                    }
                } else {
                    printf("Сначала введите полином A.\n");
                }
                break;
            case 6:
                if (Polynomial_A != NULL && Polynomial_B != NULL) {
                    Polynomial_Result = Poly_Compose(Polynomial_A, Polynomial_B);
                    if (Polynomial_Result == NULL) {
                        Print_Last_Error();
                    } else {
                        printf("Результат A(B(x)): ");
                        Poly_Print(Polynomial_Result);
                        Poly_Free(Polynomial_Result);
                    }
                } else {
                    printf("Сначала введите полиномы A и B.\n");
                }
                break;
            case 7:
                if (Polynomial_A != NULL) {
                    printf("Введите скаляр: ");
                    if (Current_Type == Int_Field_Info()) {
                        int Scalar = 0;
                        if (scanf("%d", &Scalar) != 1) {
                            Clear_Input_Buffer();
                            printf("Некорректный скаляр.\n");
                            break;
                        }
                        Polynomial_Result = Poly_Multiply_Scalar(Polynomial_A, &Scalar);
                    } else {
                        double Scalar = 0.0;
                        if (scanf("%lf", &Scalar) != 1) {
                            Clear_Input_Buffer();
                            printf("Некорректный скаляр.\n");
                            break;
                        }
                        Polynomial_Result = Poly_Multiply_Scalar(Polynomial_A, &Scalar);
                    }
                    if (Polynomial_Result == NULL) {
                        Print_Last_Error();
                    } else {
                        printf("Результат: ");
                        Poly_Print(Polynomial_Result);
                        Poly_Free(Polynomial_Result);
                    }
                } else {
                    printf("Сначала введите полином A.\n");
                }
                break;
            case 8:
                Run_Tests();
                break;
            case 9:
                if (Polynomial_A != NULL) {
                    printf("A = ");
                    Poly_Print(Polynomial_A);
                } else {
                    printf("A не задан.\n");
                }
                if (Polynomial_B != NULL) {
                    printf("B = ");
                    Poly_Print(Polynomial_B);
                } else {
                    printf("B не задан.\n");
                }
                break;
            case 10:
                if (Polynomial_A != NULL) {
                    Polynomial_Result = Poly_Derivative(Polynomial_A);
                    if (Polynomial_Result != NULL) {
                        printf("A' = ");
                        Poly_Print(Polynomial_Result);
                        Poly_Free(Polynomial_Result);
                    } else {
                        Print_Last_Error();
                    }
                } else {
                    printf("Сначала введите полином A.\n");
                }
                break;
            case 0:
                Continue_Running = 0;
                break;
            default:
                printf("Неверная команда.\n");
                break;
        }
    }

    Poly_Free(Polynomial_A);
    Poly_Free(Polynomial_B);
    return 0;
}
