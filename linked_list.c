
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct list_node list_node;
struct list_node {
	list_node * next;
	int val;
};

typedef struct list_head list_head;
struct list_head {
	list_node * head;
	list_node *tail;
	// Essentially an allocator DA
	void * memory_resource;
	size_t count;
	size_t capacity;
};

void list_insert(list_head *restrict list, int num)
{
	// Dynamically allocate new resource.
	if (list->count + 1 > list->capacity) {
		list->capacity = (size_t)((double)(2 + list->capacity) * 1.4);
		[[maybe_unused]]
		void *const p = realloc(list->memory_resource, sizeof (list_node) * list->capacity);
		// NOTE: This can invalidate all pointers if relocated.
		// This is why we do the assert to make sure the memory did not move.
		assert(p && p == list->memory_resource);
		list->memory_resource = p;
	}
	list_node * new = (void *)(((unsigned char *)list->memory_resource) + (list->count * sizeof (*new)));
	*new = (list_node){0};
	new->val = num;
	++list->count;

	// No head. FeelsBadMan
	if (list->head == nullptr) {
		list->head = new;
		list->tail = list->head;
		return;
	}

	// Insert rear B)
	if (list->tail->val < num) {
		list->tail->next = new;
		return;
	}

	// Insert front
	if (list->head->val >= num) {
		new->next = list->head;
		list->head = new;
		return;
	}

	// Insert search
	new->next = list->head;
	list_node *cur = new->next;
	while (cur) {
		if (cur->val >= num) {
			new->next->next = new;
			new->next = cur;
			return;
		}
		new->next = cur;
		cur = cur->next;
	}
}

int main(const int argc, const char *argv [static const restrict argc])
{
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
    if (file == NULL) {
		fprintf(stderr, "Could not open file %s.\n", path);
		exit(1);
	}

	const size_t cap = (size_t)atol(arg) + 1;

    list_head list = {nullptr, nullptr, malloc(sizeof (list_node) * cap), 0, cap};
	// unsigned char list_buffer[sizeof (list_node) * cap];
	//    list_head list = {nullptr, nullptr, list_buffer, 0, cap};
    char buf[16] = {0};
    while (fgets(buf, sizeof(buf), file) != NULL) {
        int num;
        sscanf(buf, "%d", &num);
        list_insert(&list, num);
    }
    fclose(file);

    list_node * cur = list.head;
	if (cur) {
		while (cur->next) {
			list_node * next = cur->next;
			if (next->val < cur->val) {
				printf("list isn't sorted\n");
				exit(1);
			}
			cur = next;
		}
	}

    // cur = list.head;
    // while (cur != NULL) {
    //     list_node * next = cur->next;
    //     free(cur);
    //     cur = next;
    // }

	free(list.memory_resource);

    printf("success\n");
    return 0;
}
