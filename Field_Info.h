#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct Field_Info {
    const char* Type_Name;
    size_t Size;
    void (*Zero)(void* Destination);
    void (*Add)(const void* Left, const void* Right, void* Result);
    void (*Multiply)(const void* Left, const void* Right, void* Result);
    void (*Print)(const void* Value);
    int (*Scan)(void* Destination);
    void (*From_Int)(void* Destination, int Value);
} Field_Info;

#endif
