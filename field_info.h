#ifndef FIELD_INFO_H_INCLUDED
#define FIELD_INFO_H_INCLUDED

#include <stddef.h>

typedef struct Fieldinfo {
    size_t elem_size;
    void (*print)(const void* elem);
    int (*compare)(const void* a, const void* b);
    void (*square)(const void* scr, void* dst);
    void (*reverse)(const void* scr, void* dst);
    void (*copy) (const void* scr, void* dst);
}Fieldinfo;

#endif // FIELD_INFO_H_INCLUDED

