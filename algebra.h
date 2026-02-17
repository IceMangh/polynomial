#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <stddef.h>

// Типы указателей на функции
typedef void (*BinaryOperationPtr)(const void* a, const void* b, void* result);
typedef void (*ZeroFunctionPtr)(void* element);       // Инициализация нулем
typedef void (*PrintFunctionPtr)(const void* element); // Вывод на экран

typedef struct Algebra {
    BinaryOperationPtr add;
    BinaryOperationPtr multiply;
    ZeroFunctionPtr zero;       // Новое поле: нейтральный элемент сложения
    PrintFunctionPtr print;     // Новое поле: отображение
    size_t elementSize;         // Размер элемента в байтах
} Algebra;

#endif