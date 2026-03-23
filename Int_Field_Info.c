#include "Scalars.h"
#include <stdio.h>

static void Int_Zero(void* Destination) {
    *(int*)Destination = 0;
}

static void Int_Add(const void* Left, const void* Right, void* Result) {
    *(int*)Result = *(const int*)Left + *(const int*)Right;
}

static void Int_Multiply(const void* Left, const void* Right, void* Result) {
    *(int*)Result = *(const int*)Left * *(const int*)Right;
}

static void Int_From_Int(void* Destination, int Value) {
    *(int*)Destination = Value;
}

static void Int_Print(const void* Value) {
    printf("%d", *(const int*)Value);
}

static int Int_Scan(void* Destination) {
    return scanf("%d", (int*)Destination) == 1;
}

const Field_Info* Int_Field_Info(void) {
    static const Field_Info Info = {
            "Integer",
            sizeof(int),
            Int_Zero,
            Int_Add,
            Int_Multiply,
            Int_Print,
            Int_Scan,
            Int_From_Int
    };
    return &Info;
}
