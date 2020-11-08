#include "list.h"								

struct ListNode {
	elem_t data				= 0;
	int32_t prev			= 0;
	int32_t next			= 0;
	NodeStatus node_status	= FREE;
};

struct List {
	ListNode* buffer	= NULL;
	int32_t head		= 0;
	int32_t tail		= 0;
	int32_t free		= 0;
	size_t capacity		= 0;
	size_t size			= 0;
};

List* NewList(size_t capacity)
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

		list->buffer[list->free].data = value;
		list->buffer[list->free].next = list->head;
		list->buffer[list->free].prev = 0;
		list->buffer[list->free].node_status = USED;
		list->buffer[list->head].prev = list->free;
		list->head = list->free;
		list->free = temp_free_idx;
	}

	ASSERT(list)
}

 int32_t findIdx(List* list, int32_t value)
{
	assert(list != NULL);
	ASSERT(list)

	int idx = list->head;
	for (int i = 0; i < value - 1; ++i)
	{
		idx = list->buffer[idx].next;
	}

	return idx;

	ASSERT(list)
}

 void pushInIdx(List* list, int64_t value, int32_t idx)
 {
	 assert(list);
	 ASSERT(list)

	 REALLOCATION

	 int32_t real_idx = findIdx(list, idx);

	 ++list->size;

	 int32_t temp_free_idx = list->buffer[list->free].next;

	 list->buffer[list->free].node_status = USED;
	 list->buffer[list->free].data = value;
	 list->buffer[list->free].prev = list->buffer[real_idx].prev;
	 list->buffer[list->free].next = real_idx;
	 list->buffer[list->buffer[real_idx].prev].next = list->free;
	 list->buffer[real_idx].prev = list->free;
	 list->free = temp_free_idx;

	 ASSERT(list)
 }

 void pushAfterIdx(List* list, int64_t value, int32_t idx)
 {
	 assert(list != NULL);
	 ASSERT(list)

	 REALLOCATION

	 pushInIdx(list, value, idx + 1);

	 ASSERT(list)
 }

 void pushBeforeIdx(List* list, int64_t value, int32_t idx)
 {
	 assert(list != NULL);
	 ASSERT(list)

	 REALLOCATION

	 pushInIdx(list, value, idx - 1);

	 ASSERT(list)
 }

 void pushTail(List* list, int64_t value)
 {
	 assert(list != NULL);
	 ASSERT(list)

	 REALLOCATION

	 ++list->size;

	 int32_t temp_free_idx = list->buffer[list->free].next;

	 list->buffer[list->free].data = value;
	 list->buffer[list->free].prev = list->tail;
	 list->buffer[list->free].next = 0;
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

 void popInIdx(List* list, int32_t idx)
 {
	 assert(list != NULL);
	 ASSERT(list)

	 --list->size;

	 int32_t real_idx = findIdx(list, idx);

	 list->buffer[list->buffer[real_idx].prev].next = list->buffer[real_idx].next;
	 list->buffer[list->buffer[real_idx].next].prev = list->buffer[real_idx].prev;

	 deleteNode(list, real_idx);

	 ASSERT(list)
 }

 elem_t getValue(List* list, int32_t idx)
 {
	 assert(list != NULL);
	 ASSERT(list)

	 int32_t real_idx = findIdx(list, idx);

	 return list->buffer[real_idx].data;

	 ASSERT(list)
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
	 ListNode* temp = (ListNode*)realloc(list->buffer, list->capacity * INCREASE_FACTOR);
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
		 fprintf(log_file, "Next[%d]:%d\n", i,list->buffer[i].next);
		 fprintf(log_file, "Prev[%d]:%d\n", i, list->buffer[i].prev);
	 }

	 fclose(log_file);
 }

 void logicSort(List* list)
 {
	 elem_t* buffer = (elem_t*)calloc(list->size + 1, sizeof(elem_t));
	 size_t live_index = list->head;
	 for (size_t i = 1; i <= list->size && live_index != 0; ++i)
	 {
		 buffer[i] = list->buffer[live_index].data;
		 live_index = list->buffer[live_index].next;
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

	 size_t live_index = list->head;
	 while (live_index != list->tail)
	 {
		 if (list->buffer[live_index].next != 0)
		 {
			 fprintf(GraphFile, "edge[color=black]\n\"%p\"->\"%p\";\n", &list->buffer[live_index], &list->buffer[list->buffer[live_index].next]);
			 live_index = list->buffer[live_index].next;
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

 void listDoubleDump(List* list)
 {
	 if (list->capacity < 100)
	 {
		 listDumpReal(list);
	 }
	 if (list->size < 100)
	 {
		 listDumpLogical(list);
	 }
 }
