#ifndef POLY_H
#define POLY_H

#include <stddef.h>

typedef struct FieldInfo {
    const char* typeName;
    size_t size;
    void (*zero)(void* dest);

    void (*add)(const void* a, const void* b, void* res); // Сложение
    void (*mult)(const void* a, const void* b, void* res);// Умножение
    void (*print)(const void* a);                   // Вывод
    void (*scan)(void* dest);                       // Ввод
    void (*from_int)(void* dest, int value);
} FieldInfo;

typedef struct Polynomial {
    void* coefficients;
    int degree;
    const FieldInfo* type;
} Polynomial;



Polynomial* Poly_Create(int degree, const FieldInfo* type);
Polynomial* Poly_Enter(Polynomial* a);
void Poly_Free(Polynomial* p);

void Poly_Set(Polynomial* p, int index, const void* value);
void* Poly_Get(const Polynomial* p, int index);

Polynomial* Poly_Add(const Polynomial* a, const Polynomial* b);
Polynomial* Poly_Mult(const Polynomial* a, const Polynomial* b);
Polynomial* Poly_MultScalar(const Polynomial* a, const void* scalar);

void Poly_Eval(const Polynomial* p, const void* x, void* result);
Polynomial* Poly_Compose(const Polynomial* p, const Polynomial* q);
Polynomial* Poly_Diff(const Polynomial* p);

// Вывод
void Poly_Print(const Polynomial* p);

#endif