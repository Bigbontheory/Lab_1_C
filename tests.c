#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "dynamic_array.h"
#include "double_field_info.h"
#include "complex_field_info.h"

void double_negate(const void* src, void* dst) {
    *(double*)dst = -(*(double*)src);
}

int is_greater_than_five(const void* elem) {
    return (*(double*)elem) > 5.0;
}

void test_init_destroy() {
    DynamicArray arr;
    dyn_array_init(&arr, get_double_field_info());
    assert(arr.size == 0);
    assert(arr.capacity == 0); 
    assert(arr.data == NULL);

    dyn_array_destroy(&arr);
    assert(arr.size == 0);
    assert(arr.data == NULL);
    printf("[OK] Init/Destroy\n");
}

void test_capacity_growth() {
    DynamicArray arr;
    dyn_array_init(&arr, get_double_field_info());

    double val = 1.0;
    for (int i = 0; i < 5; i++) {
        dyn_array_push_back(&arr, &val);
    }

    assert(arr.size == 5);
    assert(arr.capacity == 8);
    assert(arr.data != NULL);

    dyn_array_destroy(&arr);
    printf("[OK] Capacity growth (realloc)\n");
}

void test_get_set_bounds() {
    DynamicArray arr;
    dyn_array_init(&arr, get_double_field_info());

    double v1 = 10.0, v2 = 20.0;
    dyn_array_push_back(&arr, &v1);
   
    dyn_array_set_element(&arr, 0, &v2);
    double* res = (double*)dyn_array_get_element(&arr, 0);
    assert(*res == 20.0);
 
    assert(dyn_array_get_element(&arr, 5) == NULL);
    assert(dyn_array_get_element(&arr, -1) == NULL);

    dyn_array_destroy(&arr);
    printf("[OK] Get/Set and bounds\n");
}


void test_copy() {
    DynamicArray src, dst;
    dyn_array_init(&src, get_double_field_info());

    double val = 42.0;
    dyn_array_push_back(&src, &val);

    dyn_array_copy(&src, &dst);

    assert(dst.size == src.size);
    assert(*(double*)dyn_array_get_element(&dst, 0) == 42.0);

    double newVal = 100.0;
    dyn_array_set_element(&src, 0, &newVal);
    assert(*(double*)dyn_array_get_element(&dst, 0) == 42.0);

    dyn_array_destroy(&src);
    dyn_array_destroy(&dst);
    printf("[OK] Deep copy\n");
}


void test_map() {
    DynamicArray src, dst;
    dyn_array_init(&src, get_double_field_info());
    dyn_array_init(&dst, get_double_field_info());

    double v = 10.0;
    dyn_array_push_back(&src, &v);

    dyn_array_map(&src, &dst, double_negate);

    assert(*(double*)dyn_array_get_element(&dst, 0) == -10.0);

    dyn_array_destroy(&src);
    dyn_array_destroy(&dst);
    printf("[OK] Map (Transform)\n");
}

void test_where() {
    DynamicArray src, res;
    dyn_array_init(&src, get_double_field_info());
    dyn_array_init(&res, get_double_field_info());

    double v1 = 2.0, v2 = 8.0, v3 = 4.0;
    dyn_array_push_back(&src, &v1);
    dyn_array_push_back(&src, &v2);
    dyn_array_push_back(&src, &v3);

    dyn_array_where(&src, &res, is_greater_than_five);

    assert(res.size == 1);
    assert(*(double*)dyn_array_get_element(&res, 0) == 8.0);

    dyn_array_destroy(&src);
    dyn_array_destroy(&res);
    printf("[OK] Where (Filter)\n");
}

void test_concat() {
    DynamicArray a, b, res;
    dyn_array_init(&a, get_double_field_info());
    dyn_array_init(&b, get_double_field_info());
    dyn_array_init(&res, get_double_field_info());

    double v1 = 1.0, v2 = 2.0;
    dyn_array_push_back(&a, &v1);
    dyn_array_push_back(&b, &v2);

    dyn_array_concatenate(&a, &b, &res);

    assert(res.size == 2);
    assert(*(double*)dyn_array_get_element(&res, 0) == 1.0);
    assert(*(double*)dyn_array_get_element(&res, 1) == 2.0);

    dyn_array_destroy(&a);
    dyn_array_destroy(&b);
    dyn_array_destroy(&res);
    printf("[OK] Concatenation\n");
}

void test_sort_complex() {
    DynamicArray arr;
    dyn_array_init(&arr, get_double_field_info());

    double vals[] = { 5.0, 1.0, 9.0, 3.0, 7.0 };
    for (int i = 0; i < 5; i++) dyn_array_push_back(&arr, &vals[i]);

    sort_array(&arr);

    for (int i = 0; i < 4; i++) {
        double current = *(double*)dyn_array_get_element(&arr, i);
        double next = *(double*)dyn_array_get_element(&arr, i + 1);
        assert(current <= next);
    }

    dyn_array_destroy(&arr);
    printf("[OK] QuickSort\n");
}

void test_swap() {
    DynamicArray arr;
    dyn_array_init(&arr, get_double_field_info());
    double v1 = 1.0, v2 = 2.0;
    dyn_array_push_back(&arr, &v1);
    dyn_array_push_back(&arr, &v2);

    dyn_array_swap(&arr, 0, 1);

    assert(*(double*)dyn_array_get_element(&arr, 0) == 2.0);
    assert(*(double*)dyn_array_get_element(&arr, 1) == 1.0);

    dyn_array_destroy(&arr);
    printf("[OK] Swap\n");
}

void test_complex_work() {
    DynamicArray arr;
    dyn_array_init(&arr, get_complex_field_info());

    Complex c = { 3.0, 4.0 }; // Модуль 5
    dyn_array_push_back(&arr, &c);

    Complex* res = (Complex*)dyn_array_get_element(&arr, 0);
    assert(res->ReZ == 3.0 && res->ImZ == 4.0);

    dyn_array_destroy(&arr);
    printf("[OK] Complex type support\n");
}

int main() {
    printf("--- STARTING ALL TESTS ---\n");
    test_init_destroy();
    test_capacity_growth();
    test_get_set_bounds();
    test_copy();
    test_map();
    test_where();
    test_concat();
    test_swap();
    test_sort_complex();
    test_complex_work();
    printf("--- ALL TESTS PASSED SUCCESSFULLY! ---\n");
    return 0;
}
