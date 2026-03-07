#include <stdio.h>
#include <stdlib.h>


#include "field_info.h"
#include "complex_field_info.c"
#include "double_field_info.c"
#include "dynamic_array.c"


void massive_input(DynamicArray* arr);
void print_main_menu();
int select_array(int array_cnt, DynamicArray* arrays[]);

int main() {
    DynamicArray* arrays[10];
    int array_cnt = 0;
    int choice = -1;

    printf("=== DYNAMIC ARRAY MANAGER ===\n");

    while (choice != 0) {
        print_main_menu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                if (array_cnt >= 10) { printf("Error: Storage full!\n"); break; }
                int type;
                printf("\nSelect Type:\n1. Double\n2. Complex\nChoice: ");
                scanf("%d", &type);

                Fieldinfo* info = (type == 1) ? get_double_field_info() : get_complex_field_info();
                arrays[array_cnt] = (DynamicArray*)malloc(sizeof(DynamicArray));
                dyn_array_init(arrays[array_cnt], info);

                printf("Array #%d created.\n", ++array_cnt);
                break;
            }

            case 2: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1) massive_input(arrays[idx]);
                break;
            }

            case 3: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1) {
                    int pos;
                    printf("Enter index: ");
                    scanf("%d", &pos);
                    void* elem = dyn_array_get_element(arrays[idx], pos);
                    if (elem) {
                        printf("Value at [%d]: ", pos);
                        arrays[idx]->info->print(elem);
                        printf("\n");
                    } else {
                        printf("Error: Index out of bounds!\n");
                    }
                }
                break;
            }

            case 4: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1) {
                    int pos;
                    printf("Index to edit: ");
                    scanf("%d", &pos);
                    if (pos >= 0 && pos < arrays[idx]->size) {
                        void* buf = malloc(arrays[idx]->info->elem_size);
                        if (arrays[idx]->info->elem_size == sizeof(double)) {
                            printf("New double: "); scanf("%lf", (double*)buf);
                        } else {
                            Complex* c = (Complex*)buf;
                            printf("New Complex (Re Im): "); scanf("%lf %lf", &c->ReZ, &c->ImZ);
                        }
                        dyn_array_set_element(arrays[idx], pos, buf);
                        free(buf);
                        printf("Element updated.\n");
                    } else {
                        printf("Invalid index!\n");
                    }
                }
                break;
            }

            case 5: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1) {
                    printf("\n--- Array #%d ---\n", idx + 1);
                    printf("Size: %d | Capacity: %d\n",
                           dyn_array_get_size(arrays[idx]),
                           dyn_array_get_capacity(arrays[idx]));
                    printf("Content: ");
                    dyn_array_print(arrays[idx]);
                }
                break;
            }

            case 6: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1) {
                    if (arrays[idx]->size > 0) {
                        dyn_array_pop_back(arrays[idx]);
                        printf("Last element removed.\n");
                    } else printf("Array is already empty.\n");
                }
                break;
            }

            case 7: {
                int idx = select_array(array_cnt, arrays);
                if (idx != -1 && array_cnt < 10) {
                    arrays[array_cnt] = (DynamicArray*)malloc(sizeof(DynamicArray));
                    dyn_array_copy(arrays[idx], arrays[array_cnt]);
                    printf("Array #%d is a copy of #%d.\n", ++array_cnt, idx + 1);
                }
                break;
            }

            case 8: {
                int idx = select_array(array_cnt, arrays);
                if (idx == -1) break;

                printf("\n--- Processing Array #%d ---\n", idx + 1);
                printf("1. Sort (Hoare)  2. Square (Map)  3. Reverse (Map)  4. Filter (Where)\nChoice: ");
                int op; scanf("%d", &op);

                if (op == 1) {
                    sort_array(arrays[idx]);
                    printf("Sorted.\n");
                } else if (op >= 2 && op <= 4) {
                    if (array_cnt >= 10) { printf("No space for result!\n"); break; }
                    arrays[array_cnt] = (DynamicArray*)malloc(sizeof(DynamicArray));
                    dyn_array_init(arrays[array_cnt], arrays[idx]->info);

                    if (op == 2) dyn_array_map(arrays[idx], arrays[array_cnt], arrays[idx]->info->square);
                    else if (op == 3) dyn_array_map(arrays[idx], arrays[array_cnt], arrays[idx]->info->reverse);
                    else if (op == 4) dyn_array_where(arrays[idx], arrays[array_cnt], arrays[idx]->info->is_fit);

                    printf("Success! Result saved as Array #%d.\n", ++array_cnt);
                }
                break;
            }

            case 9: {
                if (array_cnt < 2) { printf("Need 2 arrays!\n"); break; }
                if (array_cnt >= 10) { printf("Full!\n"); break; }
                int i1, i2;
                printf("Select 2 indices (e.g., 1 2): ");
                scanf("%d %d", &i1, &i2);
                i1--; i2--;

                if (i1 >= 0 && i2 >= 0 && i1 < array_cnt && i2 < array_cnt) {
                    if (arrays[i1]->info != arrays[i2]->info) {
                        printf("Error: Type mismatch!\n");
                    } else {
                        arrays[array_cnt] = (DynamicArray*)malloc(sizeof(DynamicArray));
                        dyn_array_init(arrays[array_cnt], arrays[i1]->info);
                        dyn_array_concatenate(arrays[i1], arrays[i2], arrays[array_cnt]);
                        printf("Concatenated as Array #%d.\n", ++array_cnt);
                    }
                }
                break;
            }

            case 0: break;
        }
    }


    for (int i = 0; i < array_cnt; i++) {
        dyn_array_dinit(arrays[i]);
        free(arrays[i]);
    }
    printf("Memory cleared. Goodbye!\n");
    return 0;
}



void print_main_menu() {
    printf("\n--- MAIN MENU ---\n");
    printf("1. Create    2. Input    3. Get      4. Set\n");
    printf("5. Print     6. Pop      7. Copy     8. Proc\n");
    printf("9. Concat    0. Exit\nChoice: ");
}

int select_array(int array_cnt, DynamicArray* arrays[]) {
    if (array_cnt <= 0) { printf("No arrays exist yet!\n"); return -1; }
    printf("Select array index (1-%d): ", array_cnt);
    int idx;
    if (scanf("%d", &idx) != 1) { while(getchar() != '\n'); return -1; }
    if (idx < 1 || idx > array_cnt) { printf("Invalid index!\n"); return -1; }
    return idx - 1;
}

void massive_input(DynamicArray* arr) {
    if (!arr) return;
    void* buffer = malloc(arr->info->elem_size);
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
    while(getchar() != '\n');
    free(buffer);
}
