#include <stdio.h>
#include <stdlib.h>
#include "field_info.h"
#include "complex_field_info.h"
#include "double_field_info.h"
#include "dynamic_array.h"

#define MAX_ARRAYS 10

void create_array(DynamicArray* arrays[], int* count);
void input_data(DynamicArray* arrays[], int count);
void get_element(DynamicArray* arrays[], int count);
void set_element(DynamicArray* arrays[], int count);
void show_array(DynamicArray* arrays[], int count);
void pop_element(DynamicArray* arrays[], int count);
void copy_array(DynamicArray* arrays[], int* count);
void process_array(DynamicArray* arrays[], int* count);
void concat_arrays(DynamicArray* arrays[], int* count);
void print_main_menu();
int select_array(int count);
void clear_input_buffer();

int main() {
    DynamicArray* arrays[MAX_ARRAYS];
    int array_cnt = 0;
    int choice = -1;

    printf("=== DYNAMIC ARRAY MANAGER ===\n");

    while (choice != 0) {
        print_main_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        switch (choice) {
            case 1: create_array(arrays, &array_cnt); break;
            case 2: input_data(arrays, array_cnt);    break;
            case 3: get_element(arrays, array_cnt);   break;
            case 4: set_element(arrays, array_cnt);   break;
            case 5: show_array(arrays, array_cnt);    break;
            case 6: pop_element(arrays, array_cnt);   break;
            case 7: copy_array(arrays, &array_cnt);   break;
            case 8: process_array(arrays, &array_cnt); break;
            case 9: concat_arrays(arrays, &array_cnt); break;
            case 0: break;
            default: printf("Unknown command.\n");
        }
    }

    for (int i = 0; i < array_cnt; i++) {
        dyn_array_destroy(arrays[i]);
        free(arrays[i]);
    }
    printf("Memory cleared. Goodbye!\n");
    return 0;
}

void clear_input_buffer() {
    while (getchar() != '\n');
}

void print_main_menu() {
    printf("\n--- MAIN MENU ---\n");
    printf("1. Create    2. Input    3. Get      4. Set\n");
    printf("5. Print     6. Pop      7. Copy     8. Proc\n");
    printf("9. Concat    0. Exit\nChoice: ");
}

int select_array(int count) {
    if (count <= 0) { printf("No arrays exist yet!\n"); return -1; }
    printf("Select array index (1-%d): ", count);
    int idx;
    if (scanf("%d", &idx) != 1) { clear_input_buffer(); return -1; }
    if (idx < 1 || idx > count) { printf("Invalid index!\n"); return -1; }
    return idx - 1;
}

void create_array(DynamicArray* arrays[], int* count) {
    if (*count >= MAX_ARRAYS) { printf("Error: Storage full!\n"); return; }
    int type;
    printf("\nSelect Type:\n1. Double\n2. Complex\nChoice: ");
    if (scanf("%d", &type) != 1) { clear_input_buffer(); return; }

    const Fieldinfo* info = (type == 1) ? get_double_field_info() : get_complex_field_info();
    arrays[*count] = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (arrays[*count]) {
        dyn_array_init(arrays[*count], info);
        printf("Array #%d created.\n", ++(*count));
    }
}

void input_data(DynamicArray* arrays[], int count) {
    int idx = select_array(count);
    if (idx == -1) return;

    DynamicArray* arr = arrays[idx];
    void* buffer = malloc(arr->info->elem_size);
    if (!buffer) return;

    printf("\n--- MASSIVE INPUT ---\nEnter values (letter to stop):\n");
    while (1) {
        printf("[%d] -> ", arr->size);
        if (arr->info->elem_size == sizeof(double)) {
            if (scanf("%lf", (double*)buffer) != 1) break;
        } else {
            Complex* c = (Complex*)buffer;
            printf("(Re Im): ");
            if (scanf("%lf %lf", &c->ReZ, &c->ImZ) != 2) break;
        }
        dyn_array_push_back(arr, buffer);
    }
    clear_input_buffer();
    free(buffer);
}

void get_element(DynamicArray* arrays[], int count) {
    int idx = select_array(count);
    if (idx == -1) return;

    int pos;
    printf("Enter index: ");
    if (scanf("%d", &pos) == 1) {
        void* elem = dyn_array_get_element(arrays[idx], pos);
        if (elem) {
            printf("Value at [%d]: ", pos);
            arrays[idx]->info->print(elem);
            printf("\n");
        } else {
            printf("Error: Index out of bounds!\n");
        }
    } else clear_input_buffer();
}

void set_element(DynamicArray* arrays[], int count) {
    int idx = select_array(count);
    if (idx == -1) return;

    int pos;
    printf("Index to edit: ");
    if (scanf("%d", &pos) == 1) {
        if (pos >= 0 && pos < arrays[idx]->size) {
            void* buf = malloc(arrays[idx]->info->elem_size);
            if (!buf) return;

            if (arrays[idx]->info->elem_size == sizeof(double)) {
                printf("New double: ");
                if (scanf("%lf", (double*)buf) == 1) {
                    dyn_array_set_element(arrays[idx], pos, buf);
                    printf("Element updated.\n");
                }
            } else {
                Complex* c = (Complex*)buf;
                printf("New Complex (Re Im): ");
                if (scanf("%lf %lf", &c->ReZ, &c->ImZ) == 2) {
                    dyn_array_set_element(arrays[idx], pos, buf);
                    printf("Element updated.\n");
                }
            }
            free(buf);
        } else {
            printf("Invalid index!\n");
        }
    } else clear_input_buffer();
}

void show_array(DynamicArray* arrays[], int count) {
    int idx = select_array(count);
    if (idx != -1) {
        printf("\n--- Array #%d ---\n", idx + 1);
        printf("Size: %d | Capacity: %d\n", arrays[idx]->size, arrays[idx]->capacity);
        printf("Content: ");
        dyn_array_print(arrays[idx]);
    }
}

void pop_element(DynamicArray* arrays[], int count) {
    int idx = select_array(count);
    if (idx != -1) {
        if (arrays[idx]->size > 0) {
            dyn_array_pop_back(arrays[idx]);
            printf("Last element removed.\n");
        } else {
            printf("Array is already empty.\n");
        }
    }
}

void copy_array(DynamicArray* arrays[], int* count) {
    int idx = select_array(*count);
    if (idx != -1 && *count < MAX_ARRAYS) {
        arrays[*count] = (DynamicArray*)malloc(sizeof(DynamicArray));
        if (arrays[*count]) {
            dyn_array_copy(arrays[idx], arrays[*count]);
            printf("Array #%d is a copy of #%d.\n", ++(*count), idx + 1);
        }
    }
}

void process_array(DynamicArray* arrays[], int* count) {
    int idx = select_array(*count);
    if (idx == -1) return;

    printf("\n--- Processing Array #%d ---\n", idx + 1);
    printf("1. Sort 2. Square (Map)  3. Reverse (Map)  4. Filter (Where)\nChoice: ");
    int op;
    if (scanf("%d", &op) != 1) { clear_input_buffer(); return; }

    if (op == 1) {
        sort_array(arrays[idx]);
        printf("Sorted.\n");
        return;
    }

    if (*count >= MAX_ARRAYS) { printf("No space for result!\n"); return; }

    arrays[*count] = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (!arrays[*count]) return;
    
    dyn_array_init(arrays[*count], arrays[idx]->info);

    if (op == 2) {
        dyn_array_map(arrays[idx], arrays[*count], arrays[idx]->info->square);
    } else if (op == 3) {
        dyn_array_map(arrays[idx], arrays[*count], arrays[idx]->info->reverse);
    } else if (op == 4) {
        int cond;
        if (arrays[idx]->info->elem_size == sizeof(double)) {
            printf("1. Positive  2. Negative\nChoice: ");
            if (scanf("%d", &cond) == 1) {
                dyn_array_where(arrays[idx], arrays[*count], (cond == 1) ? is_positive_double : is_negative_double);
            } else clear_input_buffer();
        } else {
            printf("1. Modulus > 1  2. Modulus < 1\nChoice: ");
            if (scanf("%d", &cond) == 1) {
                dyn_array_where(arrays[idx], arrays[*count], (cond == 1) ? mod_gt1_complex : mod_lt1_complex);
            } else clear_input_buffer();
        }
    }
    printf("Success! Result saved as Array #%d.\n", ++(*count));
}

void concat_arrays(DynamicArray* arrays[], int* count) {
    if (*count < 2 || *count >= MAX_ARRAYS) return;
    
    int i1, i2;
    printf("Select 2 indices (e.g., 1 2): ");
    if (scanf("%d %d", &i1, &i2) == 2) {
        i1--; i2--;
        if (i1 >= 0 && i2 >= 0 && i1 < *count && i2 < *count) {
            if (arrays[i1]->info == arrays[i2]->info) {
                arrays[*count] = (DynamicArray*)malloc(sizeof(DynamicArray));
                if (arrays[*count]) {
                    dyn_array_init(arrays[*count], arrays[i1]->info);
                    dyn_array_concatenate(arrays[i1], arrays[i2], arrays[*count]);
                    printf("Concatenated as Array #%d.\n", ++(*count));
                }
            } else printf("Error: Type mismatch!\n");
        } else printf("Invalid indices!\n");
    } else clear_input_buffer();
}
