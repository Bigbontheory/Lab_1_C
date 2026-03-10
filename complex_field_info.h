#ifndef COMPLEX_FIELD_INFO_H_INCLUDED
#define COMPLEX_FIELD_INFO_H_INCLUDED

#include "field_info.h"

typedef struct Complex
{
    double ReZ;
    double ImZ;
}Complex;

int mod_gt1_complex(const void* elem); // module greater then 1
int mod_lt1_complex(const void* elem); // module less then 1

const Fieldinfo* get_complex_field_info();
#endif // COMPLEX_FIELD_INFO_H_INCLUDED
