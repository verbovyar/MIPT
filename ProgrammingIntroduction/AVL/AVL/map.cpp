#include "map.h"

void writeStrings(Map* map, char* str1, char* str2)
{
	map->str1 = (char*)calloc(strlen(str1), sizeof(char));
	strcpy(map->str1, str1);
	map->str2 = (char*)calloc(strlen(str2), sizeof(char));
	strcpy(map->str2, str2);
}