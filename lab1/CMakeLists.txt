cmake_minimum_required(VERSION 3.21)
project(lab1 C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES zad2/main.c)

add_executable(lab1 zad2/main.c zad1/library.c zad1/library.h)
