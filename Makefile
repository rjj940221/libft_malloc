ifeq ($(HOSTTYPE),)
	HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif

NAME		=	libft_malloc_$(HOSTTYPE.so)
INC			=	-I ./libft
LIBS		=	-lft -lpthread -L libft -lft
CFLAGS		=	-Wall -Wextra -Werror -share $(INC)
LFLAGS		=	-l  $(INC) $(LIBS)
CC			=	gcc
LD			=	gcc

INCLUDE		=	libft_malloc.h
SRC			=	libft_free.c libft_malloc.c libft_malloc_suport.c \
				libft_realloc.c
OBJ			=	./build/libft_free.o ./build/libft_malloc.o \
				./build/libft_malloc_suport.o ./build/libft_realloc.o

all: $(NAME)

./build/libft_free.o : libft_free.c
	@mkdir -p $(dir build)
	$(CC) $(CFLAGS) -c libft_free.c -o ./build/libft_free.o

./build/libft_malloc.o : libft_malloc.c
	@mkdir -p $(dir build)
	$(CC) $(CFLAGS) -c libft_malloc.c -o ./build/libft_malloc.o

./build/libft_malloc_suport.o : libft_malloc_suport.c
	@mkdir -p $(dir build)
	$(CC) $(CFLAGS) -c libft_malloc_suport.c -o ./build/libft_malloc_suport.o

./build/libft_realloc.o : libft_realloc.c
	@mkdir -p $(dir build)
	$(CC) $(CFLAGS) -c libft_realloc.c -o ./build/libft_realloc.o

$(NAME): $(OBJ)
	@echo "\033[92m    LD    $(NAME)\033[0m"
	@$(LD) $^ $(LFLAGS) -o $(NAME)

clean:
	@echo "\033[92m    RM    object files\033[0m"
	@rm -rf build

fclean: clean
	@echo "\033[92m    RM    $(NAME)\033[0m"
	@rm -f $(NAME)
	@rm -rf $(NAME).dSYM

re: fclean all
