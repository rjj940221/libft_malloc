ifeq ($(HOSTTYPE),)
	HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

NAME		=	libft_malloc_$(HOSTTYPE).so
INC			=	-I ./libft
LIBS		=	-L libft -lft
CFLAGS		=	-Wall -Wextra -Werror -g3 -fPIC $(INC)
LFLAGS		=	-lpthread -fPIC -shared  $(INC) $(LIBS)
CC			=	gcc
LD			=	gcc

INCLUDE		=	libft_malloc.h
SRC			=	libft_free.c libft_malloc.c libft_malloc_suport.c \
				libft_realloc.c libft_malloc_destruct.c
OBJ			=	./build/libft_free.o ./build/libft_malloc.o \
				./build/libft_malloc_suport.o ./build/libft_realloc.o\
				./build/libft_malloc_destruct.o

all: lft $(NAME)

./build/libft_free.o : libft_free.c
	@mkdir -p  build
	$(CC) $(CFLAGS) -c libft_free.c -o ./build/libft_free.o

./build/libft_malloc.o : libft_malloc.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c libft_malloc.c -o ./build/libft_malloc.o

./build/libft_malloc_suport.o : libft_malloc_suport.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c libft_malloc_suport.c -o ./build/libft_malloc_suport.o

./build/libft_realloc.o : libft_realloc.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c libft_realloc.c -o ./build/libft_realloc.o

./build/libft_malloc_destruct.o : libft_malloc_destruct.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c libft_malloc_destruct.c -o ./build/libft_malloc_destruct.o

$(NAME): $(OBJ)
	@echo "\033[92m    LD    $(NAME)\033[0m"
	@$(LD) $^ $(LFLAGS) -o $(NAME)
	@rm -f libft_malloc.so
	ln -s $(NAME) libft_malloc.so

clean:
	@echo "\033[92m    RM    object files\033[0m"
	@rm -rf build
	@make -s -C libft clean

fclean: clean
	@echo "\033[92m    RM    $(NAME)\033[0m"
	@rm -f libft_malloc.so
	@rm -f $(NAME)
	@rm -rf $(NAME).dSYM
	@make -s -C libft fclean

re: fclean all

lft:
	@make -s -C libft all
