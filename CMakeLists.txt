cmake_minimum_required(VERSION 4.0)
project(mnogochlen)

set(CMAKE_CXX_STANDARD 20)

add_executable(mnogochlen main.c
        scalars.h
        double_fielf_info.c
        poly.h
        poly.c
        poly_test.h
        poly_test.c
        int_field_info.c)
