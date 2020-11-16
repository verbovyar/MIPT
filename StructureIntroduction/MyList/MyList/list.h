#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

const int INCREASE_FACTOR = 2;

struct List;
typedef int elem_t;

#ifdef LIST_DEBUG
#define ASSERT(list) if (listOk(list) != NO_ERROR) {listDump; assert(!Read log file);}
#else
#define ASSERT(list) ;
#endif // LIST_DEBUG

#define REALLOCATION if (list->size == list->capacity)				\
						{											\
							printf("your list need to realloc!\n");	\
							reallocation(list);						\
						}

enum ListError {
	NO_ERROR,
	ERROR
};

enum NodeStatus {
	FREE,
	USED
};

List* NewList			(size_t capacity);
ListError listOk		(List* list);
void reallocation		(List* list);
void listDump			(List* list);
void listDumpReal		(List* list);
void listDumpLogical	(List* list);
void freeList			(List* list);
void deleteNode			(List* list, int32_t idx);

int32_t pushBeforeIdx	(List* list, int64_t value, int32_t idx);
void pushAfterIdx		(List* list, int64_t value, int32_t idx);
void pushHead			(List* list, int64_t value);
void pushTail			(List* list, int64_t value);

void popInIdx			(List* list, int32_t idx);
void popHead			(List* list);
void popTail			(List* list);

int32_t findIdx			(List* list, int32_t value);
elem_t getValue			(List* list, int32_t idx);