cmake_minimum_required(VERSION 3.21)
project(SysOpy C)

set(CMAKE_C_STANDARD 99)

add_library(SysOpy library.c main.c)
