//
// Created by Robert JONES on 2017/06/12.
//

#include "libft_malloc.h"

int main()
{
	void *ptr = "asdf";
	show_alloc_mem();
	realloc(ptr,20);
}