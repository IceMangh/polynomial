#include "Poly.h"
#include "Scalars.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void Test_Integer(void) {
    printf("=== Тесты для Integer ===\n");
    // Create + Set/Get
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Int_Field_Info());
        assert(Polynomial_Value != NULL);
        int Value = 5;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = -3;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 7;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        assert(*(int*)Poly_Get(Polynomial_Value, 0) == 5);
        assert(*(int*)Poly_Get(Polynomial_Value, 1) == -3);
        Poly_Free(Polynomial_Value);
    }
    // Add
    {
        Polynomial* Left = Poly_Create(1, Int_Field_Info()); // 3x + 2
        int Value = 2;
        assert(Poly_Set(Left, 0, &Value) == Poly_Status_Ok);
        Value = 3;
        assert(Poly_Set(Left, 1, &Value) == Poly_Status_Ok);

        Polynomial* Right = Poly_Create(1, Int_Field_Info()); // x + 5
        Value = 5;
        assert(Poly_Set(Right, 0, &Value) == Poly_Status_Ok);
        Value = 1;
        assert(Poly_Set(Right, 1, &Value) == Poly_Status_Ok);

        Polynomial* Sum = Poly_Add(Left, Right);
        assert(Sum->Degree == 1);
        assert(*(int*)Poly_Get(Sum, 0) == 7);
        assert(*(int*)Poly_Get(Sum, 1) == 4);
        Poly_Free(Left);
        Poly_Free(Right);
        Poly_Free(Sum);
    }
    // Mult
    {
        Polynomial* Left = Poly_Create(1, Int_Field_Info()); // 3x + 2
        int Value = 2;
        assert(Poly_Set(Left, 0, &Value) == Poly_Status_Ok);
        Value = 3;
        assert(Poly_Set(Left, 1, &Value) == Poly_Status_Ok);

        Polynomial* Right = Poly_Create(1, Int_Field_Info()); // x + 5
        Value = 5;
        assert(Poly_Set(Right, 0, &Value) == Poly_Status_Ok);
        Value = 1;
        assert(Poly_Set(Right, 1, &Value) == Poly_Status_Ok);

        Polynomial* Product = Poly_Multiply(Left, Right);
        assert(*(int*)Poly_Get(Product, 0) == 10);
        assert(*(int*)Poly_Get(Product, 1) == 17);
        assert(*(int*)Poly_Get(Product, 2) == 3);
        Poly_Free(Left);
        Poly_Free(Right);
        Poly_Free(Product);
    }
    // MultScalar
    {
        Polynomial* Polynomial_Value = Poly_Create(1, Int_Field_Info()); // 3x + 2
        int Value = 2;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = 3;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        int Scalar = 4;
        Polynomial* Result = Poly_Multiply_Scalar(Polynomial_Value, &Scalar);
        assert(*(int*)Poly_Get(Result, 0) == 8);
        assert(*(int*)Poly_Get(Result, 1) == 12);
        Poly_Free(Polynomial_Value);
        Poly_Free(Result);
    }
    // Eval
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Int_Field_Info()); // 2x² + 3x + 1
        int Value = 1;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = 3;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 2;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        int X_Value = 2;
        int Result = 0;
        assert(Poly_Evaluate(Polynomial_Value, &X_Value, &Result) == Poly_Status_Ok);
        assert(Result == 15);
        Poly_Free(Polynomial_Value);
    }
    // Compose
    {
        Polynomial* Outer = Poly_Create(1, Int_Field_Info()); // 2x + 1
        int Value = 1;
        assert(Poly_Set(Outer, 0, &Value) == Poly_Status_Ok);
        Value = 2;
        assert(Poly_Set(Outer, 1, &Value) == Poly_Status_Ok);

        Polynomial* Inner = Poly_Create(1, Int_Field_Info()); // x + 3
        Value = 3;
        assert(Poly_Set(Inner, 0, &Value) == Poly_Status_Ok);
        Value = 1;
        assert(Poly_Set(Inner, 1, &Value) == Poly_Status_Ok);

        Polynomial* Composition = Poly_Compose(Outer, Inner);
        assert(Composition->Degree == 1);
        assert(*(int*)Poly_Get(Composition, 0) == 7);
        Poly_Free(Outer);
        Poly_Free(Inner);
        Poly_Free(Composition);
    }
    // Derivative
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Int_Field_Info()); // 4x² + 5x + 6
        int Value = 6;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = 5;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 4;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        Polynomial* Derivative = Poly_Derivative(Polynomial_Value);
        assert(Derivative->Degree == 1);
        assert(*(int*)Poly_Get(Derivative, 0) == 5);
        assert(*(int*)Poly_Get(Derivative, 1) == 8);
        Poly_Free(Polynomial_Value);
        Poly_Free(Derivative);
    }
    printf("✓ Integer — все тесты пройдены!\n\n");
}

static void Test_Double(void) {
    printf("=== Тесты для Double ===\n");
    // Create + Set/Get + MultScalar
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Double_Field_Info());
        double Value = 1.5;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = -2.75;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 4.0;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        double Scalar = 2.0;
        Polynomial* Result = Poly_Multiply_Scalar(Polynomial_Value, &Scalar);
        assert(fabs(*(double*)Poly_Get(Result, 0) - 3.0) < 1e-9);
        Poly_Free(Polynomial_Value);
        Poly_Free(Result);
    }
    // Add
    {
        Polynomial* Left = Poly_Create(1, Double_Field_Info()); // 2.5x + 1.2
        double Value = 1.2;
        assert(Poly_Set(Left, 0, &Value) == Poly_Status_Ok);
        Value = 2.5;
        assert(Poly_Set(Left, 1, &Value) == Poly_Status_Ok);

        Polynomial* Right = Poly_Create(1, Double_Field_Info()); // 1.5x + 3.8
        Value = 3.8;
        assert(Poly_Set(Right, 0, &Value) == Poly_Status_Ok);
        Value = 1.5;
        assert(Poly_Set(Right, 1, &Value) == Poly_Status_Ok);

        Polynomial* Sum = Poly_Add(Left, Right);
        assert(fabs(*(double*)Poly_Get(Sum, 0) - 5.0) < 1e-9);
        assert(fabs(*(double*)Poly_Get(Sum, 1) - 4.0) < 1e-9);
        Poly_Free(Left);
        Poly_Free(Right);
        Poly_Free(Sum);
    }
    // Eval
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Double_Field_Info()); // 2x² + 3x + 1
        double Value = 1.0;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = 3.0;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 2.0;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        double X_Value = 2.0;
        double Result = 0.0;
        assert(Poly_Evaluate(Polynomial_Value, &X_Value, &Result) == Poly_Status_Ok);
        assert(fabs(Result - 15.0) < 1e-9);
        Poly_Free(Polynomial_Value);
    }
    // Compose
    {
        Polynomial* Outer = Poly_Create(1, Double_Field_Info()); // 2x + 1
        double Value = 1.0;
        assert(Poly_Set(Outer, 0, &Value) == Poly_Status_Ok);
        Value = 2.0;
        assert(Poly_Set(Outer, 1, &Value) == Poly_Status_Ok);

        Polynomial* Inner = Poly_Create(1, Double_Field_Info()); // x + 3
        Value = 3.0;
        assert(Poly_Set(Inner, 0, &Value) == Poly_Status_Ok);
        Value = 1.0;
        assert(Poly_Set(Inner, 1, &Value) == Poly_Status_Ok);

        Polynomial* Composition = Poly_Compose(Outer, Inner);
        assert(Composition->Degree == 1);
        assert(fabs(*(double*)Poly_Get(Composition, 0) - 7.0) < 1e-9);
        Poly_Free(Outer);
        Poly_Free(Inner);
        Poly_Free(Composition);
    }
    // Derivative
    {
        Polynomial* Polynomial_Value = Poly_Create(2, Double_Field_Info()); // 3x² + 2x + 1
        double Value = 1.0;
        assert(Poly_Set(Polynomial_Value, 0, &Value) == Poly_Status_Ok);
        Value = 2.0;
        assert(Poly_Set(Polynomial_Value, 1, &Value) == Poly_Status_Ok);
        Value = 3.0;
        assert(Poly_Set(Polynomial_Value, 2, &Value) == Poly_Status_Ok);
        Polynomial* Derivative = Poly_Derivative(Polynomial_Value);
        assert(Derivative->Degree == 1);
        assert(fabs(*(double*)Poly_Get(Derivative, 0) - 2.0) < 1e-9);
        assert(fabs(*(double*)Poly_Get(Derivative, 1) - 6.0) < 1e-9);
        Poly_Free(Polynomial_Value);
        Poly_Free(Derivative);
    }
    printf("✓ Double — все тесты пройдены!\n\n");
}

void Run_Tests(void) {
    Test_Integer();
    Test_Double();
}
