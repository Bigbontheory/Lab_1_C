#ifndef DOUBLE_FIELD_INFO_H
#define DOUBLE_FIELD_INFO_H

#include "field_info.h"

int is_positive_double(const void* elem);
int is_negative_double(const void* elem);

const Fieldinfo* get_double_field_info();

#endif
