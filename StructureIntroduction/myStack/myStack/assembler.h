#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "mystack.h"

struct comandsList {
  char* buffer = NULL;
  size_t comands_count = 0;
  size_t buffer_size = 0;
  char** lines = NULL;
};

struct Label {
	char* lab_name = NULL;
	size_t PC = 0;
};

enum READERROR { 
	READ_ERROR, 
	NON_READ_ERROR 
};

void interpritation();
READERROR readcomands(comandsList* comands, char* name);
size_t getFileSize(const char* file_name);
size_t getLinesCount(comandsList* comands);
void linesConstruct(comandsList* comands);
void writeToBin(char* code_buffer, int PC);
bool isLabel(const char* token);
Label* labelConstr();
void addNumber(char* code_buffer, const char* token, Label* label);
bool myIsAlphabet(const char symbol);
void writeLableName(Label* label);