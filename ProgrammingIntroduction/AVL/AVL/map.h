#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

struct Map {
	char* str1 = NULL;
	char* str2 = NULL;
};

void writeStrings(Map* map, char* str1, char* str2);
