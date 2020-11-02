#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "stack.h"

struct CPU {
	myStack* stck       = NULL;
	myStack* call_stack = NULL;
	char* comand_buffer = NULL;
	size_t buffer_ofs = 0;
	bool isWorking = true;
	int    OFS = 0;
	double rax = 0;
	double rbx = 0;
	double rcx = 0;
	double rdx = 0;
};

void   getAnswer   ();
size_t getFileSize (const char* file_name);
void   getFile     (char* name, CPU* cpu);
