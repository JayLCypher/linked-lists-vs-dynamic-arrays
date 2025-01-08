.PHONY: all test clean


MOLD_STACK_SIZE := -Wl,-z -Wl,stack-size=1048576
DEBUG := -fanalyzer -fsanitize=address,bounds,undefined -g3 -DDEBUG
RELEASE := -O3 -DNDEBUG
CFLAGS := -std=c23 -Wall -Wextra -pedantic-errors -Wconversion -Wshadow $(RELEASE)

all: linked_list dynamic_array

linked_list: linked_list.c
	$(CC) -o $@ $^ $(CFLAGS)

dynamic_array: dynamic_array.c
	$(CC) -o $@ $^ $(CFLAGS)

