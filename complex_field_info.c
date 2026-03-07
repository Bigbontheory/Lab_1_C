#include <stdio.h>
#include <stdlib.h>
#include "complex_field_info.h"
void print_complex(const void* elem)
{
    Complex*z = (Complex*)elem;
    printf("%f + i*%f,", z-> ReZ, z-> ImZ);
}

double complex_mod_sq(const Complex* elem)
{
    const Complex* z = elem;
    double r = (z->ReZ)*(z->ReZ) + (z->ImZ)*(z->ImZ);
    return r;
}

int is_fit_complex(const void* elem) {
    if (!elem) return 0;
    const Complex* z = (const Complex*)elem;
    return complex_mod_sq(z) > 1.0;
}


int compare_complex(const void*a, const void* b)
{
    Complex* z1 = (Complex*) a;
    Complex* z2 = (Complex*) b;
    double r1 = complex_mod_sq(z1);
    double r2 = complex_mod_sq(z2);
    if (r1>r2){
        return 1;
    }
    else if(r1<r2){
        return -1;
    }
    else return 0;

}

void square_complex(const void* scr, void* dst) {
    const Complex* z1 = (const Complex*)scr;
    Complex* z2 = (Complex*)dst;
    double re = (z1->ReZ * z1->ReZ) - (z1->ImZ * z1->ImZ);
    double im = 2 * z1->ReZ * z1->ImZ;
    z2->ReZ = re;
    z2->ImZ = im;
}

void reverse_complex(const void* scr, void* dst) {
    const Complex* z = (const Complex*)scr;
    Complex* res = (Complex*)dst;
    res->ReZ = -z->ReZ;
    res->ImZ = -z->ImZ;
}

void copy_complex(const void* scr, void* dst){
    const Complex* z = (const Complex*)scr;
    Complex* res = (Complex*)dst;
    res->ReZ = z ->ReZ;
    res->ImZ = z ->ImZ;
}

Fieldinfo* COMPLEX_FIELD_INFO;
Fieldinfo* get_complex_field_info() {
    if (COMPLEX_FIELD_INFO == NULL) {
        COMPLEX_FIELD_INFO = (Fieldinfo*)malloc(sizeof(Fieldinfo));
        COMPLEX_FIELD_INFO->elem_size = sizeof(Complex);
        COMPLEX_FIELD_INFO->print = print_complex;
        COMPLEX_FIELD_INFO->compare = compare_complex;
        COMPLEX_FIELD_INFO->square = square_complex;
        COMPLEX_FIELD_INFO->reverse = reverse_complex;
        COMPLEX_FIELD_INFO->copy = copy_complex;
        COMPLEX_FIELD_INFO->is_fit = is_fit_complex;
    }
    return COMPLEX_FIELD_INFO;
}




