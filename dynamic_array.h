#ifndef DYNAMIC_ARRAY_H_INCLUDED
#define DYNAMIC_ARRAY_H_INCLUDED

#include "field_info.h"

typedef struct DynamicArray {
    void* data;
    int size;
    int capacity;
    const Fieldinfo* info;
} DynamicArray;

void dyn_array_init(DynamicArray* arr, const Fieldinfo* info);
void dyn_array_destroy(DynamicArray* arr);
void dyn_array_copy(const DynamicArray* src, DynamicArray* dst);

void* dyn_array_get_element(const DynamicArray* arr, int index);

void dyn_array_push_back(DynamicArray* arr, const void* value);
void dyn_array_pop_back(DynamicArray* arr);
void dyn_array_set_element(DynamicArray* arr, int i, const void* value);

void dyn_array_print(const DynamicArray* arr);

void dyn_array_map(DynamicArray* arr, DynamicArray* dest,  void (*f)(const void* src, void* dst));
void dyn_array_where(const DynamicArray* src, DynamicArray* result, int (*predicate)(const void*));
void dyn_array_concatenate(const DynamicArray* left, const DynamicArray* right, DynamicArray* result);
void sort_array(DynamicArray* arr);

#endif // DYNAMIC_ARRAY_H_INCLUDED

