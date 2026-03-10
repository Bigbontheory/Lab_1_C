#include <stdlib.h>
#include <stddef.h>
#include "dynamic_array.h"
#include <stdio.h>

void dyn_array_swap(DynamicArray* arr, int i, int j) {
    if (i == j) return;
    void* temp = malloc(arr->info->elem_size);
    if (!temp) return;
    void* addr_i = (char*)arr->data + (i * arr->info->elem_size);
    void* addr_j = (char*)arr->data + (j * arr->info->elem_size);
    arr->info->copy(addr_i, temp);
    arr->info->copy(addr_j, addr_i);
    arr->info->copy(temp, addr_j);

    free(temp);
}

void quick_sort_din_array(DynamicArray* arr, int low, int high, void* pivot_buffer) {
    if (low >= high) return;
    int i = low;
    int j = high;
    void* middle_elem = (char*)arr->data + ((low + high) / 2 * arr->info->elem_size);
    arr->info->copy(middle_elem, pivot_buffer);
    while (i <= j) {
        while (arr->info->compare((char*)arr->data + i * arr->info->elem_size, pivot_buffer) < 0) i++;
        while (arr->info->compare((char*)arr->data + j * arr->info->elem_size, pivot_buffer) > 0) j--;
        if (i <= j) {
            dyn_array_swap(arr, i, j);
            i++;
            j--;
        }
    }
    if (low < j) quick_sort_din_array(arr, low, j, pivot_buffer);
    if (i < high) quick_sort_din_array(arr, i, high, pivot_buffer);
}

void dyn_array_init(DynamicArray* arr, const Fieldinfo* info) {
    if (!arr || !info) return;
    arr->info = info;
    arr->size = 0;
    arr->capacity = 0;
    arr->data = NULL;
}

void dyn_array_destroy(DynamicArray* arr){
    if (!arr) return;
    if(arr->data){
        free(arr->data);
    }
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;

}

void dyn_array_copy(const DynamicArray* src, DynamicArray* dst){
    if (!src || !dst) return;
    dyn_array_init(dst,src->info);
    if (src->size == 0) return;
    dst->data = malloc(src->size*src->info->elem_size);
    if(!dst->data) return;
    dst->size = src->size;
    dst->capacity = src->size;
    for(int i=0; i < src->size; i++)
    {
        void* src_elem = (char*)src->data +(i*src->info->elem_size);
        void* dst_elem = (char*)dst->data +(i*dst->info->elem_size);
        src->info->copy(src_elem, dst_elem);
    }
}

void* dyn_array_get_element(const DynamicArray* arr, int index) {
    if (!arr || index < 0 || index >= arr->size) {
        return NULL;
    }
    return (char*)arr->data + (index * arr->info->elem_size);
}

void dyn_array_push_back(DynamicArray* arr, const void* value){
    if(!arr || !value) return;
    if(arr->size == arr->capacity)
    {
        int new_capacity =(arr->capacity ==0)? 4 : arr->capacity*2;
        void* new_data = realloc(arr->data, new_capacity* arr->info->elem_size);
        if (new_data == NULL) return;
        arr->data = new_data;
        arr->capacity = new_capacity;
     }
    void* target =(char*)arr->data + (arr->size * arr->info->elem_size);
    arr->info->copy(value, target);
    arr->size++;
}

void dyn_array_pop_back(DynamicArray* arr){
    if(!arr || arr->size ==0) return;
    arr->size--;
}

void dyn_array_set_element(DynamicArray* arr, int i, const void* value){
    if(!arr || !value || i < 0 || i >= arr->size) return;
    void* target = (char*)arr->data + (i * arr->info->elem_size);
    arr->info->copy(value, target);
}

void dyn_array_print(const DynamicArray* arr){
    if (!arr || arr->size == 0) {
        printf("Array is empty\n");
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        void* elem = (char*)arr->data + (i * arr->info->elem_size);
        arr->info->print(elem);
        printf(" ");
    }
    printf("\n");
}

void dyn_array_map(DynamicArray* arr, DynamicArray* dest, void (*f)(const void* src, void* dst)) {
    if (!arr || !dest || !f) return;

    void* tmp = malloc(arr->info->elem_size);
    if (!tmp) return;

    for (int i = 0; i < arr->size; i++) {
        void* src_elem = (char*)arr->data + (i * arr->info->elem_size);
        f(src_elem, tmp);
        dyn_array_push_back(dest, tmp);
    }

    free(tmp);
}

void dyn_array_where(const DynamicArray* src, DynamicArray* result, int (*predicate)(const void*)) {
    if (!src || !result || !predicate) return;

    for (int i = 0; i < src->size; i++) {
        void* elem = (char*)src->data + (i * src->info->elem_size);
        if (predicate(elem)) {
            dyn_array_push_back(result, elem);
        }
    }
}

void dyn_array_concatenate(const DynamicArray* left, const DynamicArray* right, DynamicArray* result) {
    if (!left || !right || !result) return;
    if (left->info != right->info || left->info != result->info) return;
    for (int i = 0; i < left->size; i++) {
        void* item = (char*)left->data + (i * left->info->elem_size);
        dyn_array_push_back(result, item);
    }
    for (int i = 0; i < right->size; i++) {
        void* item = (char*)right->data + (i * right->info->elem_size);
        dyn_array_push_back(result, item);
    }
}

void sort_array(DynamicArray* arr) {
    if (arr == NULL || arr->data == NULL || arr->size < 2) {
        return;
    }
    void* pivot_buffer = malloc(arr->info->elem_size);
    if (!pivot_buffer) return;
    quick_sort_din_array(arr, 0, arr->size - 1, pivot_buffer);

    free(pivot_buffer);
}
