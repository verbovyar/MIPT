#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const int PASEGES_COUNT = 2;
const int MAX_STR_SIZE = 256;
const int REGISTERS_COUNT = 4;
const int MAX_BUFFER_SIZE = 1e6;
const int MAX_LABLE_ARRAY_SIZE = 1e4;

struct comandsList {
	char* buffer = NULL;
	char** lines = NULL;
	size_t comands_count = 0;
	size_t buffer_size   = 0;
	int ofs              = 0;
};

struct Label {
	char* lab_name = NULL;
	int pc = 0;
};

enum READERROR {
	READ_ERROR,
	NON_READ_ERROR
};

void getByteCode(const char* name);
Label* labelArrayConstr();
comandsList* comandsConstr();

READERROR readcomands    (comandsList* comands, const char* name);
size_t    getLinesCount  (comandsList* comands);
void      linesConstruct (comandsList* comands);

size_t getFileSize    (const char* file_name);
bool   isLabel        (const char* token);
void   addArgument    (const char* token, Label* label, int lab_idx, char* code_buffer, comandsList* comands);
void   writeLableName (const char* token, Label* label, int* lab_idx, comandsList* comands);
bool   myIsAlphabet   (const char symbol);
void   writeToBin     (char* code_buffer, int pc);