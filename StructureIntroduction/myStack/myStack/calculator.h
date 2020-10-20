#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "mystack.h"
#include "interpritation.h"

struct CPU {
	myStack* stck = NULL;
	char* comand_buffer = NULL;
	size_t buffer_size = 0;
	double rax = 0;
	double rbx = 0;
	double rcx = 0;
	double rdx = 0;
};

enum bitCode {
	PUSH = 48,
	SUMM = 49,
	SQRT = 54,
	HULT = 57,
	OUT  = 97
};

void getAnswer();
size_t GetFileSize(const char* file_name);
void getFile(char* name, CPU* cpu);
