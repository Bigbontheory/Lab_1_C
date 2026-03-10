cmake_minimum_required (VERSION 3.10)
project (MyLab1 C)

add_executable (my_prog 
    main.c
    dynamic_array.c 
    complex_field_info.c 
    double_field_info.c)


add_executable (full_tests 
    tests.c 
    dynamic_array.c 
    complex_field_info.c 
    double_field_info.c
)



