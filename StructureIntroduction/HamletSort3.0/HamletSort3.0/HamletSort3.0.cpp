#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

//
//-------------------------
//functions prototips
//-------------------------
//

int findFileSize(FILE* fileName);

int findCountOfStr(char* buffer, int sizeOfFile);

void makeLines(char* buffer, char const** lines, int sizeOfFile, int countOfStr);

void sortLines(void* point, int countOfStr);

void isSymbols(void* point, int countOfStr, int index2);

void myStrCmp(void* point, int index2, int k, int j);

bool myIsAlpha(char symbol);

void mySwap(void* point, int index2);

void writeTxtFile(char const** lines, int countOfStr);

void runUnitTests();

void checkCorrect(bool value, int line);

//
//-------------------------
// main (add buffer and read file in one buffer)
//-------------------------
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

//
//-------------------------
// function find size of input file
//-------------------------
//

int findFileSize(FILE* fileName)
{
    assert(fileName != NULL);

    fseek(fileName, 0, SEEK_END);
    const int sizeOfFile = ftell(fileName);
    fseek(fileName, 0, SEEK_SET);

    return sizeOfFile;
}

//
//-------------------------
// find count of strings if input file
//-------------------------
//

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

//
//-------------------------
// function make string (from buffer)
//-------------------------
//

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

//
//-------------------------
// strings sortings
//-------------------------
//

void sortLines(void* point, int countOfStr)
{
    assert(point != NULL);

    char const** lines = (char const**)point; //type
    for (int index1 = 0; index1 < countOfStr; ++index1)
    {
        for (int index2 = countOfStr - 1; index2 > index1; --index2)
        {
            isSymbols(lines, countOfStr, index2);
        }
    }
}

//
//-------------------------
// find for the first differend letters
//-------------------------
//

void isSymbols(void* point, int countOfStr, int index2)
{
    char const** lines = (char const**)point; //type
    int i = 0;
    while (*(lines[index2 - 1] + i) == *(lines[index2] + i))
    {
        ++i;
    }
    int k = i;
    int j = i;
    while (!myIsAlpha(*(lines[index2 - 1] + k)))
    {
        ++k;
    }
    while (!myIsAlpha(*(lines[index2] + j)))
    {
        ++j;
    }

    myStrCmp(lines, index2, k, j);
}

//
//-------------------------
//string compare
//-------------------------
//

void myStrCmp(void* point, int index2, int k, int j)
{
    assert(point != NULL);

    char const** lines = (char const**)point; //type
    if (*(lines[index2 - 1] + k) > *(lines[index2] + j))
    {
        mySwap(lines, index2);
    }
}

//
//-------------------------
//check symbol to alphabet
//-------------------------
//

bool myIsAlpha(char symbol)
{
    if ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//
//-------------------------
//change strings
//-------------------------
//

void mySwap(void* point, int index2)
{
    assert(point != NULL);

    char const** lines = (char const**)point; //type
    char const* temp = lines[index2 - 1];
    lines[index2 - 1] = lines[index2];
    lines[index2] = temp;
}

//
//-------------------------
//write sort output file in txt
//-------------------------
//

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

//
//-------------------------
//unit testing
//-------------------------
//

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
    char const* array3[] = { "hello", "world" };
    char const* temp1 = array3[0];
    char const* temp2 = array3[1];
    int strCount = 2;
    int strIndex = 1;

    mySwap(array3, strIndex);

    value1 = (array3[0] == temp2);
    value2 = (array3[1] == temp1);
    checkCorrect(value1, __LINE__);
    checkCorrect(value2, __LINE__);
}

//
//-------------------------
// check correct or incorrect unittest
//-------------------------
//

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