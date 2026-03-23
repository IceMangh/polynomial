#ifndef POLY_H
#define POLY_H

#include "Field_Info.h"

typedef enum Poly_Status {
    Poly_Status_Ok = 0,
    Poly_Status_Invalid_Argument,
    Poly_Status_Invalid_Degree,
    Poly_Status_Type_Mismatch,
    Poly_Status_Index_Out_Of_Range,
    Poly_Status_Allocation_Failed,
    Poly_Status_Input_Failed
} Poly_Status;

typedef struct Polynomial {
    void* Coefficients;
    int Degree;
    const Field_Info* Type;
} Polynomial;

Polynomial* Poly_Create(int Degree, const Field_Info* Type);
Poly_Status Poly_Input(Polynomial* Polynomial_Value);
void Poly_Free(Polynomial* Polynomial_Value);

Poly_Status Poly_Set(Polynomial* Polynomial_Value, int Index, const void* Value);
void* Poly_Get(const Polynomial* Polynomial_Value, int Index);

Polynomial* Poly_Add(const Polynomial* Left, const Polynomial* Right);
Polynomial* Poly_Multiply(const Polynomial* Left, const Polynomial* Right);
Polynomial* Poly_Multiply_Scalar(const Polynomial* Polynomial_Value, const void* Scalar);

Poly_Status Poly_Evaluate(const Polynomial* Polynomial_Value, const void* X_Value, void* Result);
Polynomial* Poly_Compose(const Polynomial* Outer, const Polynomial* Inner);
Polynomial* Poly_Derivative(const Polynomial* Polynomial_Value);

Poly_Status Poly_Print(const Polynomial* Polynomial_Value);

Poly_Status Poly_Get_Last_Status(void);
const char* Poly_Get_Last_Error(void);
void Poly_Clear_Last_Error(void);

#endif
