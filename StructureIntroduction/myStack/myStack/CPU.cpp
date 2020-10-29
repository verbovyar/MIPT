#define _CRT_SECURE_NO_WARNINGS

#include "CPU.h"
#include <ctype.h>

CPU my_cpu = {};
void getAnswer()
{
	my_cpu.stck = newStack(5);
	char name[] = "assembler_code.bin";

	getFile(name, &my_cpu);

	int OFS = 0;
	while (my_cpu.isWorking)
	{
		int comand = my_cpu.comand_buffer[OFS];
		switch (comand)
		{
			#define DEF_CMD(name, num, arg, code)	\
			case(num):								\
			{										\
				code								\
				break;								\
			}
			#include "comands.h"
		}
		//printf("TOP:%lf\n", top(my_cpu.stck));
		//printf("RAX:%lf\n", my_cpu.rax);
		//printf("RBX:%lf\n", my_cpu.rbx);
		//printf("RCX:%lf\n", my_cpu.rcx);
		//printf("RDX:%lf\n", my_cpu.rdx);
		++OFS;
	}
}

size_t GetFileSize(const char* file_name)
{
	assert(file_name != NULL);

	struct stat fileStat = {};
	stat(file_name, &fileStat);

	return fileStat.st_size;
}

void getFile(char* name, CPU* cpu)
{
	assert(cpu != NULL);

	FILE* file_descriptor = fopen(name, "r");

	cpu->buffer_ofs = getFileSize(name);
	cpu->comand_buffer = (char*)calloc(cpu->buffer_ofs + 1, sizeof(double));
	fread(cpu->comand_buffer, sizeof(double), cpu->buffer_ofs + 1, file_descriptor);

	fclose(file_descriptor);
}
