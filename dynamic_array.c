
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct dynamic_array da;
struct dynamic_array{
    int *items;
    long count;
    long capacity;
};

void da_insert(da *restrict array, int num) {
	// Preemtive resize.
    if (array->count + 1 > array->capacity) {
		array->capacity = (long)((float)(1 + array->capacity) * 1.4f);
		[[maybe_unused]]
		void *const p = realloc(array->items, sizeof (*array->items) * (unsigned)array->capacity);
		assert(p);
		array->items = p;
    }

    for (long i = 0; i != array->count; ++i) {
        if (array->items[i] >= num) {
			#if 1
			for (long j = array->count; j != i; --j) {
				array->items[j] = array->items[j-1];
			}
			#else
			memmove(&array->items[i + 1], &array->items[i], (unsigned long)(array->count - i) * sizeof (*array->items));
			#endif
			array->items[i] = num;
			++array->count;
			return;
        }
    }
	array->items[array->count++] = num;
}

int main(int argc, char **argv) {
    if (argc < 2) {
		fprintf(stderr, "Not enough arguments, required 1, found %d\n", argc-1);
		exit(1);
	}
    char path[4096] = {0};
	const char *arg = argv[1];
	assert(strlen(arg) < sizeof (path));
    strcat(path, "inputs/");
    strcat(path, arg);
    strcat(path, ".txt");

    FILE *file = fopen(path, "r");
    if (file == nullptr) {
		fprintf(stderr, "Could not open file %s.\n", path);
		exit(1);
	}

	const long cap = atol(arg) + 1;
    da array = {
        malloc(sizeof(int) * (unsigned long)(cap)),
        0,
		cap,
    };
	memset(array.items, 0, sizeof (*array.items) * (unsigned long)array.capacity);
    char buf[16] = {0};
    while (fgets(buf, sizeof(buf), file) != nullptr) {
        int num;
        sscanf(buf, "%d", &num);
        da_insert(&array, num);
    }
    fclose(file);

    for (long i = 0; i != array.count-1; ++i) {
        if (array.items[i] > array.items[i+1]) {
            printf("array isn't sorted\n");
            exit(1);
        }
    }
	free(array.items);
    printf("success\n");
    return 0;
}
