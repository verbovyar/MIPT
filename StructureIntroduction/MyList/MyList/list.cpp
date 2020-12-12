#include "list.h"								

struct ListNode {
	elem_t data = 0;
	int32_t prev = 0;
	int32_t next = 0;
	NodeStatus node_status = FREE;
};

struct List {
	ListNode* buffer = NULL;
	bool is_sorted = false;
	int32_t head = 0;
	int32_t tail = 0;
	int32_t free = 0;
	size_t capacity = 0;
	size_t size = 0;
};

List* newList(size_t capacity)
{
	List* list = (List*)calloc(1, sizeof(List));
	list->buffer = (ListNode*)calloc(capacity + 1, sizeof(ListNode));

	list->capacity = capacity;
	list->size = 0;
	list->head = 1;
	list->tail = 1;

	list->buffer[0].next = 0;
	list->buffer[0].prev = 0;

	for (int i = 1; i <= capacity; ++i)
	{
		list->buffer[i].prev = -1;
		list->buffer[i].next = i + 1;
		list->buffer[i].node_status = FREE;
	}

	list->buffer[capacity].next = 0;
	list->free = 1;

	return list;
}

void pushHead(List* list, int64_t value)
{
	assert(list != NULL);
	ASSERT(list)

	REALLOCATION

	++list->size;

	if (list->size == 1)
	{
		list->buffer[1].data = value;
		list->buffer[1].prev = 0;
		list->free = list->buffer[1].next;
		list->buffer[1].node_status = USED;
	}
	else
	{
		int32_t temp_free_idx = list->buffer[list->free].next;

		ListNode* free_elem = &list->buffer[list->free];

		free_elem->data = value;
		free_elem->next = list->head;
		free_elem->prev = 0;
		list->buffer[list->head].prev = list->free;
		list->head = list->free;
		list->free = temp_free_idx;
	}

	ASSERT(list)
}

int32_t findIdx(List* list, int32_t logic_idx)
{
	assert(list != NULL);
	ASSERT(list)

	int real_idx = 0;
	if (list->is_sorted == true)
	{
		real_idx = logic_idx;
	}
	else
	{
		real_idx = list->head;
		for (int i = 0; i < logic_idx - 1; ++i)
		{
			real_idx = list->buffer[real_idx].next;
		}
	}

	ASSERT(list)

	return real_idx;
}

int32_t pushBeforeIdx(List* list, int64_t value, int32_t real_idx)
{
	assert(list);
	ASSERT(list)

	REALLOCATION

	++list->size;

	int32_t temp_free_idx = list->buffer[list->free].next;
	int32_t push_idx = list->free;

	ListNode* push_elem = &list->buffer[push_idx];

	push_elem->data = value;
	push_elem->prev = list->buffer[real_idx].prev;
	push_elem->next = real_idx;
	list->buffer[list->buffer[real_idx].prev].next = list->free;
	list->buffer[real_idx].prev = list->free;
	list->free = temp_free_idx;
	list->is_sorted = false;

	ASSERT(list)

	return push_idx;
}

void pushAfterIdx(List* list, int64_t value, int32_t idx)
{
	assert(list != NULL);
	ASSERT(list)

	REALLOCATION

	pushBeforeIdx(list, value, idx - 1);

	ASSERT(list)
}

void pushTail(List* list, int64_t value)
{
	assert(list != NULL);
	ASSERT(list)

	REALLOCATION

	++list->size;

	int32_t temp_free_idx = list->buffer[list->free].next;

	ListNode* free_elem = &list->buffer[list->free];

	free_elem->data = value;
	free_elem->prev = list->tail;
	free_elem->next = 0;
	list->buffer[list->tail].next = list->free;
	list->tail = list->free;
	list->free = temp_free_idx;

	ASSERT(list)
}

void popHead(List* list)
{
	assert(list != NULL);
	ASSERT(list)

	list->buffer[list->buffer[list->head].next].prev = 0;
	int32_t idx = list->head;
	list->head = list->buffer[list->head].next;

	deleteNode(list, idx);

	ASSERT(list)
}

void popTail(List* list)
{
	assert(list != NULL);
	ASSERT(list)

	list->buffer[list->buffer[list->tail].prev].next = 0;
	int32_t idx = list->tail;
	list->tail = list->buffer[list->tail].prev;

	deleteNode(list, idx);

	ASSERT(list)
}

void popInIdx(List* list, int32_t real_idx)
{
	assert(list != NULL);
	ASSERT(list)

	--list->size;

	list->buffer[list->buffer[real_idx].prev].next = list->buffer[real_idx].next;
	list->buffer[list->buffer[real_idx].next].prev = list->buffer[real_idx].prev;

	deleteNode(list, real_idx);

	ASSERT(list)
}

elem_t getValue(List* list, int32_t real_idx)
{
	assert(list != NULL);
	ASSERT(list)

	return list->buffer[real_idx].data;
}

void deleteNode(List* list, int32_t idx)
{
	assert(list != NULL);
	ASSERT(list)

	--list->size;

	list->buffer[idx].next = list->free;
	list->buffer[idx].prev = -1;
	list->buffer[idx].data = 0;
	list->free = idx;

	ASSERT(list)
}

void reallocation(List* list)
{
	list->buffer = (ListNode*)realloc(list->buffer, list->capacity * INCREASE_FACTOR);
}

void freeList(List* list)
{
	assert(list != NULL);
	ASSERT(list)

	free(list->buffer);
	list->buffer = NULL;
	list->head = 1;
	list->tail = 1;

	list->free = 0;

	list->size = 0;
	list->capacity = 0;

	ASSERT(list)
}

ListError listOk(List* list)
{
	if (list == NULL)
	{
		return ERROR;
	}

	if (list->capacity < list->size)
	{
		return ERROR;
	}

	if (list->capacity < 0)
	{
		return ERROR;
	}

	return NO_ERROR;
}

void listDump(List* list)
{
	FILE* log_file = fopen("logfile.txt", "w");

	fprintf(log_file, "Capacity:%d\n", list->capacity);
	fprintf(log_file, "Size:%d\n", list->size);
	fprintf(log_file, "Tail:%d\n", list->tail);
	fprintf(log_file, "Head:%d\n", list->head);

	for (int i = 0; i < list->capacity; ++i)
	{
		fprintf(log_file, "Value[%d]:%d\n", i, list->buffer[i].data);
		fprintf(log_file, "Next[%d]:%d\n", i, list->buffer[i].next);
		fprintf(log_file, "Prev[%d]:%d\n", i, list->buffer[i].prev);
	}

	fclose(log_file);
}

void logicSort(List* list)
{
	elem_t* buffer = (elem_t*)calloc(list->size + 1, sizeof(elem_t));
	size_t real_idx = list->head;
	for (size_t i = 1; i <= list->size && real_idx != 0; ++i)
	{
		buffer[i] = list->buffer[real_idx].data;
		real_idx = list->buffer[real_idx].next;
	}
	for (size_t i = 1; i <= list->size; ++i)
	{
		list->buffer[i].data = buffer[i];
		list->buffer[i].prev = i - 1;
		list->buffer[i].next = i + 1;
	}

	list->buffer[list->size].next = 0;
	list->free = list->size + 1;

	list->head = 1;
	list->tail = list->size;

	for (size_t i = list->size + 1; i <= list->capacity; ++i)
	{
		list->buffer[i].next = i + 1;
		list->buffer[i].prev = -1;
	}

	list->buffer[list->capacity].next = 0;
	list->is_sorted = true;
	free(buffer);
}

void listDumpLogical(List* list)
{
	FILE* GraphFile = fopen("list_graph.txt", "w");

	logicSort(list);

	fprintf(GraphFile, "digraph G{\n");
	fprintf(GraphFile, "node [shape=\"record\", style=\"filled\", color=\"#008000\", fillcolor=\"#C0FFC0\"];\n");

	fprintf(GraphFile, "\"%p\" [color=\"#000080\", fillcolor=\"#C0C0FF\", label=\"{HEAD}\"];\n", &list->buffer[0]);

	for (size_t i = 1; i <= list->size; ++i)
	{
		if (list->buffer[i].prev != -1)
		{
			fprintf(GraphFile, "\"%p\" [label=\"{%d|{%d|%u|%d}}\"]\n",
				&list->buffer[i], list->buffer[i].data, list->buffer[i].prev, i, list->buffer[i].next);
			if (i != list->size)
			{
				if (i != 1)
				{
					fprintf(GraphFile, "\"%p\"->\"%p\";\n", &list->buffer[i], &list->buffer[list->buffer[i].prev]);
				}
				else
				{
					fprintf(GraphFile, "\"%p\"->\"%p\";\n", &list->buffer[list->buffer[i].prev], &list->buffer[i]);
				}
			}
			fprintf(GraphFile, "\"%p\"->\"%p\";\n", &list->buffer[i], &list->buffer[list->buffer[i].next]);
		}
	}
	fprintf(GraphFile, "}");

	fclose(GraphFile);

	if (list->size < 100)
	{
		system("dot -Tjpg list_graph.txt > LogicalList.jpg");
		system("start LogicalList.jpg");
	}
}

void listDumpReal(List* list)
{
	FILE* GraphFile = fopen("list_graph.txt", "w");

	fprintf(GraphFile, "digraph G{\n");
	fprintf(GraphFile, "node [shape=\"record\", style=\"filled\", color=\"#008000\", fillcolor=\"#C0FFC0\"];\n");

	for (size_t i = 1; i <= list->capacity; ++i)
	{
		if (list->buffer[i].prev == -1)
		{
			fprintf(GraphFile, "\"%p\" [fillcolor=\"#EFE28E\", label=\"{FREE|{%u|%d}}\"];\n", &list->buffer[i], i, list->buffer[i].next);
		}
		else
		{
			fprintf(GraphFile, "\"%p\" [color=\"#000080\", label=\"{%d|{%d|%u|%d}}\"];\n",
				&list->buffer[i], list->buffer[i].data, list->buffer[i].prev, i, list->buffer[i].next);
		}

		if (i < list->capacity)
		{
			fprintf(GraphFile, "edge[color=white]\n\"%p\"->\"%p\";\n", &list->buffer[i], &list->buffer[i + 1]);
		}
	}

	fprintf(GraphFile, "\"HEAD\" [color=\"#000080\", fillcolor=\"#C0C0FF\", label=\"{HEAD}\"];\n");

	fprintf(GraphFile, "edge[color=black]\n\"HEAD\"->\"%p\";\n", &list->buffer[list->head]);

	size_t real_idx = list->head;
	while (real_idx != list->tail)
	{
		if (list->buffer[real_idx].next != 0)
		{
			fprintf(GraphFile, "edge[color=black]\n\"%p\"->\"%p\";\n", &list->buffer[real_idx], &list->buffer[list->buffer[real_idx].next]);
			real_idx = list->buffer[real_idx].next;
		}
		else break;
	}

	fprintf(GraphFile, "\"%p\"->\"HEAD\"", &list->buffer[list->tail]);

	fprintf(GraphFile, "}");

	fclose(GraphFile);

	if (list->capacity < 100)
	{
		system("dot -Tjpg list_graph.txt > RealList.jpg");
		system("start RealList.jpg");
	}
}