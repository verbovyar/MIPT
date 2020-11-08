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
	 list->tail = list->buffer[list->head].prev;

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
