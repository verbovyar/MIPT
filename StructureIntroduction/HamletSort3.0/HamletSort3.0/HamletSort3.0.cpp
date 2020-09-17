#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int findFileSize(FILE* fileName)
{
    assert(fileName != NULL);

    fseek(fileName, 0, SEEK_END);
    const int sizeOfFile = ftell(fileName);
    fseek(fileName, 0, SEEK_SET);

    return sizeOfFile;
}

int findCountOfStr(char* buffer, int sizeOfFile)
{
    assert(buffer != NULL);

    int countOfStr = 1;

    for (int index = 0; index < sizeOfFile - 1; ++index)
    {
        if (buffer[index] == '\n')
        {
            ++countOfStr;
            buffer[index] = '\0';
        }
    }

    return countOfStr;
}

void makeLines(char* buffer, char** lines, int sizeOfFile, int countOfStr)
{
    assert(buffer != NULL);
    assert(lines != NULL);

    lines[0] = &buffer[0];
    int i = 0;
    for (int indexOfLine = 1; indexOfLine < countOfStr; ++indexOfLine)
    {
        while (buffer[i] != '\0')
        {
            ++i;
        }

        if (buffer[i] == '\0')
        {
            lines[indexOfLine] = &buffer[i + 1];
            ++i;
        }
    }
}

void sortLines(char** lines, int countOfStr)
{
    for (int index1 = 0; index1 < countOfStr; ++index1)
    {
        for (int index2 = countOfStr - 1; index2 > index1; --index2)
        {
            int i = 0;
            while (*(lines[index2 - 1] + i) == *(lines[index2] + i))
            {
                ++i;
            }

            if (*(lines[index2 - 1] + i) > *(lines[index2] + i))
            {
                char* temp = lines[index2 - 1];
                lines[index2 - 1] = lines[index2];
                lines[index2] = temp;
            }
        }
    }
}

void writeTxtFile(char** lines, int countOfStr)
{
    FILE* fileout = fopen("output2.txt", "w");

    for (int index = 0; index < countOfStr; ++index)
    {
        fprintf(fileout, "%s\n", lines[index]);
    }

    fclose(fileout);
}

int main()
{
    FILE* fileName = fopen("hamlet.txt", "r"); // add const value for TXT file 

    const int sizeOfFile = findFileSize(fileName);
    char* buffer = (char*)calloc(sizeOfFile + 1, sizeof(char));
    fread(buffer, sizeof(char), sizeOfFile, fileName);

    fclose(fileName);

    const int countOfStr = findCountOfStr(buffer, sizeOfFile);
    char** lines = (char**)calloc(countOfStr + 1, sizeof(char*));
    makeLines(buffer, lines, sizeOfFile, countOfStr);

    sortLines(lines, countOfStr);

    writeTxtFile(lines, countOfStr);
    
    free(buffer);
    free(lines);

    return 0;
}