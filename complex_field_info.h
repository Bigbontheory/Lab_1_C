#ifndef COMPLEX_FIELD_INFO_H_INCLUDED
#define COMPLEX_FIELD_INFO_H_INCLUDED
#include "field_info.h"
typedef struct Complex
{
    double ReZ;
    double ImZ;
}Complex;

Fieldinfo* get_complex_field_info();


#endif // COMPLEX_FIELD_INFO_H_INCLUDED
