#include "Scalars.h"
#include <stdio.h>

static void Double_Zero(void* Destination) {
    *(double*)Destination = 0.0;
}

static void Double_Add(const void* Left, const void* Right, void* Result) {
    *(double*)Result = *(const double*)Left + *(const double*)Right;
}

static void Double_Multiply(const void* Left, const void* Right, void* Result) {
    *(double*)Result = *(const double*)Left * *(const double*)Right;
}

static void Double_From_Int(void* Destination, int Value) {
    *(double*)Destination = (double)Value;
}

static void Double_Print(const void* Value) {
    printf("%.2f", *(const double*)Value);
}

static int Double_Scan(void* Destination) {
    return scanf("%lf", (double*)Destination) == 1;
}

const Field_Info* Double_Field_Info(void) {
    static const Field_Info Info = {
            "Double",
            sizeof(double),
            Double_Zero,
            Double_Add,
            Double_Multiply,
            Double_Print,
            Double_Scan,
            Double_From_Int
    };
    return &Info;
}
