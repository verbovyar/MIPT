#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

struct List;

enum NodeStatus {
	FREE,
	USED
};

List* NewList		(size_t capacity);
void freeList		(List* list);
void deleteNode		(List* list, int32_t idx);

void pushInIdx		(List* list, int64_t value, int32_t idx);
void pushAfterIdx	(List* list, int64_t value, int32_t idx);
void pushBeforeIdx	(List* list, int64_t value, int32_t idx);
void pushHead		(List* list, int64_t value);
void pushTail		(List* list, int64_t value);

void popInIdx		(List* list, int32_t idx);
void popHead		(List* list);
void popTail		(List* list);

int32_t findIdx		(List* list, int32_t value);
elem_t getValue		(List* list, int32_t idx);