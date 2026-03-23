#include "Poly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Poly_Status Last_Status = Poly_Status_Ok;
static const char* Last_Error = NULL;

static Poly_Status Set_Error(Poly_Status Status, const char* Error_Message) {
    Last_Status = Status;
    Last_Error = Error_Message;
    return Status;
}

static int Is_Field_Info_Valid(const Field_Info* Type) {
    return Type != NULL
           && Type->Type_Name != NULL
           && Type->Size > 0
           && Type->Zero != NULL
           && Type->Add != NULL
           && Type->Multiply != NULL
           && Type->Print != NULL
           && Type->Scan != NULL
           && Type->From_Int != NULL;
}

static int Is_Polynomial_Valid(const Polynomial* Polynomial_Value) {
    return Polynomial_Value != NULL
           && Polynomial_Value->Degree >= 0
           && Polynomial_Value->Coefficients != NULL
           && Is_Field_Info_Valid(Polynomial_Value->Type);
}

static void* Get_Coefficient_Pointer(const Polynomial* Polynomial_Value, int Index) {
    return (char*)Polynomial_Value->Coefficients + (size_t)Index * Polynomial_Value->Type->Size;
}

static void Initialize_With_Zero(Polynomial* Polynomial_Value) {
    for (int Index = 0; Index <= Polynomial_Value->Degree; ++Index) {
        Polynomial_Value->Type->Zero(Get_Coefficient_Pointer(Polynomial_Value, Index));
    }
}

Poly_Status Poly_Get_Last_Status(void) {
    return Last_Status;
}

const char* Poly_Get_Last_Error(void) {
    return Last_Error;
}

void Poly_Clear_Last_Error(void) {
    Last_Status = Poly_Status_Ok;
    Last_Error = NULL;
}

Polynomial* Poly_Create(int Degree, const Field_Info* Type) {
    Poly_Clear_Last_Error();
    if (Degree < 0) {
        Set_Error(Poly_Status_Invalid_Degree, "степень многочлена не может быть отрицательной");
        return NULL;
    }
    if (!Is_Field_Info_Valid(Type)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректное описание поля");
        return NULL;
    }

    Polynomial* Result = malloc(sizeof(*Result));
    if (Result == NULL) {
        Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для структуры многочлена");
        return NULL;
    }

    Result->Degree = Degree;
    Result->Type = Type;
    Result->Coefficients = calloc((size_t)Degree + 1U, Type->Size);
    if (Result->Coefficients == NULL) {
        free(Result);
        Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для коэффициентов");
        return NULL;
    }

    Initialize_With_Zero(Result);
    return Result;
}

Poly_Status Poly_Input(Polynomial* Polynomial_Value) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value)) {
        return Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для ввода");
    }

    for (int Index = Polynomial_Value->Degree; Index >= 0; --Index) {
        void* Coefficient = Get_Coefficient_Pointer(Polynomial_Value, Index);
        if (!Polynomial_Value->Type->Scan(Coefficient)) {
            return Set_Error(Poly_Status_Input_Failed, "не удалось прочитать коэффициент");
        }
    }
    return Poly_Status_Ok;
}

void Poly_Free(Polynomial* Polynomial_Value) {
    if (Polynomial_Value != NULL) {
        free(Polynomial_Value->Coefficients);
        free(Polynomial_Value);
    }
}

void* Poly_Get(const Polynomial* Polynomial_Value, int Index) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен");
        return NULL;
    }
    if (Index < 0 || Index > Polynomial_Value->Degree) {
        Set_Error(Poly_Status_Index_Out_Of_Range, "индекс коэффициента вне диапазона");
        return NULL;
    }

    return Get_Coefficient_Pointer(Polynomial_Value, Index);
}

Poly_Status Poly_Set(Polynomial* Polynomial_Value, int Index, const void* Value) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value) || Value == NULL) {
        return Set_Error(Poly_Status_Invalid_Argument, "некорректные аргументы для установки коэффициента");
    }
    if (Index < 0 || Index > Polynomial_Value->Degree) {
        return Set_Error(Poly_Status_Index_Out_Of_Range, "индекс коэффициента вне диапазона");
    }

    memcpy(Get_Coefficient_Pointer(Polynomial_Value, Index), Value, Polynomial_Value->Type->Size);
    return Poly_Status_Ok;
}

Polynomial* Poly_Add(const Polynomial* Left, const Polynomial* Right) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Left) || !Is_Polynomial_Valid(Right)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для сложения");
        return NULL;
    }
    if (Left->Type != Right->Type) {
        Set_Error(Poly_Status_Type_Mismatch, "невозможно сложить многочлены разных типов");
        return NULL;
    }

    int Maximum_Degree = (Left->Degree > Right->Degree) ? Left->Degree : Right->Degree;
    Polynomial* Result = Poly_Create(Maximum_Degree, Left->Type);
    if (Result == NULL) {
        return NULL;
    }

    void* Zero_Buffer = malloc(Left->Type->Size);
    if (Zero_Buffer == NULL) {
        Poly_Free(Result);
        Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для временного буфера");
        return NULL;
    }
    Left->Type->Zero(Zero_Buffer);

    for (int Index = 0; Index <= Maximum_Degree; ++Index) {
        void* Result_Value = Get_Coefficient_Pointer(Result, Index);
        const void* Left_Value = (Index <= Left->Degree) ? Get_Coefficient_Pointer(Left, Index) : Zero_Buffer;
        const void* Right_Value = (Index <= Right->Degree) ? Get_Coefficient_Pointer(Right, Index) : Zero_Buffer;

        Left->Type->Add(Left_Value, Right_Value, Result_Value);
    }

    free(Zero_Buffer);
    return Result;
}

Polynomial* Poly_Multiply(const Polynomial* Left, const Polynomial* Right) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Left) || !Is_Polynomial_Valid(Right)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для умножения");
        return NULL;
    }
    if (Left->Type != Right->Type) {
        Set_Error(Poly_Status_Type_Mismatch, "невозможно умножить многочлены разных типов");
        return NULL;
    }

    Polynomial* Result = Poly_Create(Left->Degree + Right->Degree, Left->Type);
    if (Result == NULL) {
        return NULL;
    }

    void* Temporary_Product = malloc(Left->Type->Size);
    void* Temporary_Sum = malloc(Left->Type->Size);
    if (Temporary_Product == NULL || Temporary_Sum == NULL) {
        free(Temporary_Product);
        free(Temporary_Sum);
        Poly_Free(Result);
        Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для временных буферов");
        return NULL;
    }

    for (int Left_Index = 0; Left_Index <= Left->Degree; ++Left_Index) {
        for (int Right_Index = 0; Right_Index <= Right->Degree; ++Right_Index) {
            void* Result_Value = Get_Coefficient_Pointer(Result, Left_Index + Right_Index);
            const void* Left_Value = Get_Coefficient_Pointer(Left, Left_Index);
            const void* Right_Value = Get_Coefficient_Pointer(Right, Right_Index);

            Left->Type->Multiply(Left_Value, Right_Value, Temporary_Product);
            Left->Type->Add(Result_Value, Temporary_Product, Temporary_Sum);
            memcpy(Result_Value, Temporary_Sum, Left->Type->Size);
        }
    }

    free(Temporary_Product);
    free(Temporary_Sum);
    return Result;
}

Polynomial* Poly_Multiply_Scalar(const Polynomial* Polynomial_Value, const void* Scalar) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value) || Scalar == NULL) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректные аргументы для умножения на скаляр");
        return NULL;
    }

    Polynomial* Result = Poly_Create(Polynomial_Value->Degree, Polynomial_Value->Type);
    if (Result == NULL) {
        return NULL;
    }

    for (int Index = 0; Index <= Polynomial_Value->Degree; ++Index) {
        Polynomial_Value->Type->Multiply(
                Get_Coefficient_Pointer(Polynomial_Value, Index),
                Scalar,
                Get_Coefficient_Pointer(Result, Index)
        );
    }

    return Result;
}

Poly_Status Poly_Evaluate(const Polynomial* Polynomial_Value, const void* X_Value, void* Result) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value) || X_Value == NULL || Result == NULL) {
        return Set_Error(Poly_Status_Invalid_Argument, "некорректные аргументы для вычисления значения");
    }

    memcpy(Result, Get_Coefficient_Pointer(Polynomial_Value, Polynomial_Value->Degree), Polynomial_Value->Type->Size);
    if (Polynomial_Value->Degree == 0) {
        return Poly_Status_Ok;
    }

    void* Temporary_Value = malloc(Polynomial_Value->Type->Size);
    if (Temporary_Value == NULL) {
        return Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для вычисления значения многочлена");
    }

    for (int Index = Polynomial_Value->Degree - 1; Index >= 0; --Index) {
        Polynomial_Value->Type->Multiply(Result, X_Value, Temporary_Value);
        Polynomial_Value->Type->Add(Temporary_Value, Get_Coefficient_Pointer(Polynomial_Value, Index), Result);
    }

    free(Temporary_Value);
    return Poly_Status_Ok;
}

Polynomial* Poly_Compose(const Polynomial* Outer, const Polynomial* Inner) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Outer) || !Is_Polynomial_Valid(Inner)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для композиции");
        return NULL;
    }
    if (Outer->Type != Inner->Type) {
        Set_Error(Poly_Status_Type_Mismatch, "невозможно построить композицию для разных типов");
        return NULL;
    }

    Polynomial* Accumulator = Poly_Create(0, Outer->Type);
    if (Accumulator == NULL) {
        return NULL;
    }
    if (Poly_Set(Accumulator, 0, Get_Coefficient_Pointer(Outer, Outer->Degree)) != Poly_Status_Ok) {
        Poly_Free(Accumulator);
        return NULL;
    }

    for (int Index = Outer->Degree - 1; Index >= 0; --Index) {
        Polynomial* Multiplied = Poly_Multiply(Accumulator, Inner);
        if (Multiplied == NULL) {
            Poly_Free(Accumulator);
            return NULL;
        }
        Poly_Free(Accumulator);
        Accumulator = Multiplied;

        Polynomial* Constant_Polynomial = Poly_Create(0, Outer->Type);
        if (Constant_Polynomial == NULL) {
            Poly_Free(Accumulator);
            return NULL;
        }
        if (Poly_Set(Constant_Polynomial, 0, Get_Coefficient_Pointer(Outer, Index)) != Poly_Status_Ok) {
            Poly_Free(Accumulator);
            Poly_Free(Constant_Polynomial);
            return NULL;
        }

        Polynomial* Summed = Poly_Add(Accumulator, Constant_Polynomial);
        Poly_Free(Accumulator);
        Poly_Free(Constant_Polynomial);
        if (Summed == NULL) {
            return NULL;
        }
        Accumulator = Summed;
    }

    return Accumulator;
}

Polynomial* Poly_Derivative(const Polynomial* Polynomial_Value) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value)) {
        Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для производной");
        return NULL;
    }

    if (Polynomial_Value->Degree == 0) {
        Polynomial* Result = Poly_Create(0, Polynomial_Value->Type);
        if (Result == NULL) {
            return NULL;
        }
        Polynomial_Value->Type->Zero(Get_Coefficient_Pointer(Result, 0));
        return Result;
    }

    Polynomial* Result = Poly_Create(Polynomial_Value->Degree - 1, Polynomial_Value->Type);
    if (Result == NULL) {
        return NULL;
    }

    void* Degree_Value = malloc(Polynomial_Value->Type->Size);
    void* Temporary_Result = malloc(Polynomial_Value->Type->Size);
    if (Degree_Value == NULL || Temporary_Result == NULL) {
        free(Degree_Value);
        free(Temporary_Result);
        Poly_Free(Result);
        Set_Error(Poly_Status_Allocation_Failed, "не удалось выделить память для вычисления производной");
        return NULL;
    }

    for (int Index = 1; Index <= Polynomial_Value->Degree; ++Index) {
        Polynomial_Value->Type->From_Int(Degree_Value, Index);
        Polynomial_Value->Type->Multiply(
                Degree_Value,
                Get_Coefficient_Pointer(Polynomial_Value, Index),
                Temporary_Result
        );
        memcpy(Get_Coefficient_Pointer(Result, Index - 1), Temporary_Result, Polynomial_Value->Type->Size);
    }

    free(Degree_Value);
    free(Temporary_Result);
    return Result;
}

Poly_Status Poly_Print(const Polynomial* Polynomial_Value) {
    Poly_Clear_Last_Error();
    if (!Is_Polynomial_Valid(Polynomial_Value)) {
        return Set_Error(Poly_Status_Invalid_Argument, "некорректный многочлен для печати");
    }

    printf("Degree %d: [ ", Polynomial_Value->Degree);
    for (int Index = Polynomial_Value->Degree; Index >= 0; --Index) {
        Polynomial_Value->Type->Print(Get_Coefficient_Pointer(Polynomial_Value, Index));
        if (Index > 0) {
            printf(" x^%d + ", Index);
        }
    }
    printf(" ]\n");
    return Poly_Status_Ok;
}
