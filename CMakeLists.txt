cmake_minimum_required(VERSION 3.7)
project(libft_malloc)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES libft_Malloc.c libft_Malloc.h main.c)
add_library(libft_malloc ${SOURCE_FILES})