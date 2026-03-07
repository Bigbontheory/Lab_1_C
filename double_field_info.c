#include <stdio.h>
#include <stdlib.h>
#include "double_field_info.h"

void print_double(const void* elem) {
    printf("%f", *((double*)elem));
}

int compare_double(const void* a, const void* b) {
    double v1 = *((double*)a);
    double v2 = *((double*)b);
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

void square_double(const void* scr, void* dst) {
    *((double*)dst) = (*((double*)scr)) * (*((double*)scr));
}

void reverse_double(const void* scr, void* dst) {
    *((double*)dst) = -(*((double*)scr));
}

void copy_double(const void* scr, void* dst) {
    *((double*)dst) = *((double*)scr);
}

int is_fit_double(const void* elem) {
    return (*((double*)elem)) > 0.0;
}

Fieldinfo* DOUBLE_FIELD_INFO = NULL;

Fieldinfo* get_double_field_info() {
    if (DOUBLE_FIELD_INFO == NULL) {
        DOUBLE_FIELD_INFO = (Fieldinfo*)malloc(sizeof(Fieldinfo));
        DOUBLE_FIELD_INFO->elem_size = sizeof(double);
        DOUBLE_FIELD_INFO->compare = compare_double;
        DOUBLE_FIELD_INFO->print = print_double;
        DOUBLE_FIELD_INFO->square = square_double;
        DOUBLE_FIELD_INFO->reverse = reverse_double;
        DOUBLE_FIELD_INFO->copy = copy_double;
        DOUBLE_FIELD_INFO->is_fit=is_fit_double;
    }
    return DOUBLE_FIELD_INFO;
}
