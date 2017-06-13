cmake_minimum_required(VERSION 3.7)
project(libft_malloc)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES libft_malloc.c libft_malloc.h main.c libft_free.c libft_realloc.c libft_malloc_suport.c)
add_library(libft_malloc ${SOURCE_FILES})