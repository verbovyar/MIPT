#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int findFileSize(FILE* fileName);

int findCountOfStr(char* buffer, int sizeOfFile);

void makeLines(char* buffer, char const** lines, int sizeOfFile, int countOfStr);

void sortLines(char const** lines, int countOfStr);

void myStrCmp(char const** lines, int countOfStr, int index2);

void mySwap(char const** lines, int index2);

void writeTxtFile(char const** lines, int countOfStr);

void runUnitTests();

void checkCorrect(bool value, int line);

//
int main()
{
    runUnitTests();

    FILE* fileName = fopen("hamlet.txt", "r"); // add const value for TXT file 

    if (fileName == NULL)
    {
        printf("READING ERROR!");
        exit(0);
    }

    const int sizeOfFile = findFileSize(fileName);
    char* buffer = (char*)calloc(sizeOfFile + 1, sizeof(char));
    fread(buffer, sizeof(char), sizeOfFile, fileName);

    fclose(fileName);

    const int countOfStr = findCountOfStr(buffer, sizeOfFile);
    char const** lines = (char const**)calloc(countOfStr + 1, sizeof(char*));
    makeLines(buffer, lines, sizeOfFile, countOfStr);

    sortLines(lines, countOfStr);

    writeTxtFile(lines, countOfStr);

    free(buffer);
    free(lines);

    printf("SUCCESSFUL ENDING");

    return 0;
}

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

void makeLines(char* buffer, char const** lines, int sizeOfFile, int countOfStr)
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

void sortLines(char const** lines, int countOfStr)
{
    assert(lines != NULL);

    for (int index1 = 0; index1 < countOfStr; ++index1)
    {
        for (int index2 = countOfStr - 1; index2 > index1; --index2)
        {
            myStrCmp(lines, countOfStr, index2);
        }
    }
}

void myStrCmp(char const** lines, int countOfStr, int index2)
{
    assert(lines != NULL);

    int i = 0;
    while (*(lines[index2 - 1] + i) == *(lines[index2] + i))
    {
        ++i;
    }

    if (*(lines[index2 - 1] + i) > * (lines[index2] + i))
    {
        mySwap(lines, index2);
    }
}

void mySwap(char const** lines, int index2)
{
    assert(lines != NULL);

    char const* temp = lines[index2 - 1];
    lines[index2 - 1] = lines[index2];
    lines[index2] = temp;
}

void writeTxtFile(char const** lines, int countOfStr)
{
    assert(lines != NULL);

    FILE* fileOut = fopen("sortHamlet.txt", "w");

    if (fileOut == NULL)
    {
        printf("WRITING ERROR");
    }

    for (int index = 0; index < countOfStr; ++index)
    {
        fprintf(fileOut, "%s\n", lines[index]);
    }

    fclose(fileOut);
}

//Testing
void runUnitTests()
{
    //findFileSize
    int count1 = NULL;
    int count2 = NULL;

    FILE* test1 = fopen("findFileSizeTest1.txt", "r");
    FILE* test2 = fopen("findFileSizeTest2.txt", "r");

    count1 = findFileSize(test1);
    count2 = findFileSize(test2);

    int realCount1 = 34;
    int realCount2 = 35;

    bool value1 = (realCount1 == count1);
    bool value2 = (realCount2 == count2);
    checkCorrect(value1, __LINE__);
    checkCorrect(value2, __LINE__);

    //findCountOfStr
    test1 = fopen("findCountOfStrTest1.txt", "r");
    test2 = fopen("findCountOfStrTest2.txt", "r");

    int size1 = 34;
    int size2 = 21;
    char* text1 = (char*)calloc(size1 + 1, sizeof(char));
    char* text2 = (char*)calloc(size2 + 1, sizeof(char));

    fread(text1, sizeof(char), size1, test1);
    fread(text2, sizeof(char), size2, test2);

    fclose(test1);
    fclose(test2);

    count1 = findCountOfStr(text1, size1);
    count2 = findCountOfStr(text2, size2);

    realCount1 = 4;
    realCount2 = 4;

    value1 = (count1 == realCount1);
    value2 = (count2 == realCount2);
    checkCorrect(value1, __LINE__);
    checkCorrect(value2, __LINE__);

    free(text1);
    free(text2);

    //mySwap
    char const* array1[] = { "hello", "world" };
    char const* temp1 = array1[0];
    char const* temp2 = array1[1];
    int strCount = 2;
    int strIndex = 1;

    mySwap(array1, strIndex);

    value1 = (array1[0] == temp2);
    value2 = (array1[1] == temp1);
    checkCorrect(value1, __LINE__);
    checkCorrect(value2, __LINE__);
}

void checkCorrect(bool value, int line)
{
    if (value)
    {
        printf("CORRECT TEST\n");
    }
    else
    {
        printf("INCORRECT TEST: LINE %d\n", line);
    }
}