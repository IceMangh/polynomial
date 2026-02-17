cmake_minimum_required(VERSION 4.0)
project(mnogochlen)

set(CMAKE_CXX_STANDARD 20)

add_executable(mnogochlen main.c
        algebra.h
        scalars.h
        scalars.c
        poly.h
        poly.c)
