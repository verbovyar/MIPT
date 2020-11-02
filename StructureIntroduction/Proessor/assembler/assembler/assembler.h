#pragma once
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct comandsList {
	char* buffer = NULL;
	size_t comands_count = 0;
	size_t buffer_size = 0;
	char** lines = NULL;
	int OFS = 0;
};

struct Label {
	char* lab_name = NULL;
	int PC = 0;
};

enum READERROR {
	READ_ERROR,
	NON_READ_ERROR
};

void interpritation();
size_t getFileSize(const char* file_name);

READERROR readcomands    (comandsList* comands, char* name);
size_t    getLinesCount  (comandsList* comands);
void      linesConstruct (comandsList* comands);

void writeToBin(char* code_buffer, int PC);
bool isLabel(const char* token);
Label* labelConstr();
void addArgument(char* code_buffer, const char* token, Label* label, int lab_ind);
bool myIsAlphabet(const char symbol);
void writeLableName(Label* label, const char* temp, int* lab_ind);
