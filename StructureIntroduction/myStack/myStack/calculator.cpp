#define _CRT_SECURE_NO_WARNINGS

#include "calculator.h"
#include <ctype.h>

CPU my_cpu = {};
void getAnswer()
{
	my_cpu.stck = newStack(5);
	char name[] = "assembler_code.txt";

	getFile(name, &my_cpu);

	for (int i = 0; i < my_cpu.buffer_size; ++i)
	{
		int comand = my_cpu.comand_buffer[i];
		switch (comand)
		{
			case(PUSH):
			{
				push(my_cpu.stck, my_cpu.comand_buffer[i + 1] - '0');
				++i;

				break;
			}
			case(SUMM):
			{
				double numb1 = top(my_cpu.stck);
				pop(my_cpu.stck);
				double numb2 = top(my_cpu.stck);
				pop(my_cpu.stck);
				push(my_cpu.stck, numb1 + numb2);

				break;
			}
			case(SQRT):
			{
				double result = sqrt(top(my_cpu.stck));
				pop(my_cpu.stck);
				push(my_cpu.stck,result);

				break;
			}
			case(HULT):
			{
				deleteStack(my_cpu.stck);

				break;
			}
			case(OUT):
			{
				FILE* file_discriptor = fopen("result.txt", "w");

				double result = top(my_cpu.stck);
				fprintf(file_discriptor, "result:%lf", result);

				fclose(file_discriptor);
			}
		}
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
	FILE* file_descriptor = fopen(name, "r");

	cpu->buffer_size = getFileSize(name);
	cpu->comand_buffer = (char*)calloc(cpu->buffer_size + 1, sizeof(double));
	fread(cpu->comand_buffer, sizeof(double), cpu->buffer_size + 1, file_descriptor);

	fclose(file_descriptor);
}
