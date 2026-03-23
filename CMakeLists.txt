cmake_minimum_required(VERSION 3.16)
project(Polynomial C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Polynomial
        Main.c
        Scalars.h
        Field_Info.h
        Double_Field_Info.c
        Poly.h
        Poly.c
        Poly_Test.h
        Poly_Test.c
        Int_Field_Info.c)
