#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "mystack.h"
#include "assembler.h"

struct CPU {
	myStack* stck = NULL;
	char* comand_buffer = NULL;
	size_t buffer_ofs = 0;
	bool isWorking = true;
	double rax = 0;
	double rbx = 0;
	double rcx = 0;
	double rdx = 0;
};

void getAnswer();
size_t GetFileSize(const char* file_name);
void getFile(char* name, CPU* cpu);
