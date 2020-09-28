#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

//
//-------------------------
//struct for lines
//-------------------------
//

struct Line {
    char* string_ = NULL;
    int len_ = 0;
};

//
//-------------------------
//functions prototips
//-------------------------
//

int getFileSize(FILE* fileName, char* file_name);

int getLinesCount(char* buffer, int buffer_size);

void convertToStringsArray(char* buffer, int file_size, int strings_count, Line* strings_array); 

void reverseSort(void* pointer, int strings_count);

bool myStrReversCmp(void* pointer, int index, int first_line_letter_position, int second_line_letter_position, int position,
                    int len1, int len2);

void sortArray(void* pointer, int countOfStr);

bool myStrCmp(void* pointer, int index, int first_line_letter_position, int second_line_letter_position, int position);

bool myIsAlphabet(char symbol);

void mySwap(void* pointer, int index);

void saveToTxtFile(Line* strings_array, int strings_count);

void runUnitTests();

void printTestResult(bool value, int line);

//
//-------------------------
// main (add buffer and read file in one buffer)
//-------------------------
//

int main()
{
    runUnitTests();

    FILE* file_descriptor = fopen("hamlet.txt", "r");

    if (file_descriptor == NULL)
    {
        printf("\nREADING ERROR!");
        return -1;
    }

    char file_name[] = "hamlet.txt";
    int file_size = getFileSize(file_descriptor, file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    file_size = fread(buffer, sizeof(char), file_size +1 , file_descriptor);

    fclose(file_descriptor);

    const int lines_count = getLinesCount(buffer, file_size);
    Line* strings_array = (Line*)calloc(lines_count + 1, sizeof(Line));
    convertToStringsArray(buffer, file_size, lines_count, strings_array);

    //reverseSort(strings_array, lines_count);

    sortArray(strings_array, lines_count);

    saveToTxtFile(strings_array, lines_count);

    free(buffer);
    free(strings_array);

    printf("SUCCESSFUL ENDING");

    return 0;
}

//
//-------------------------
// function find size of input file
//-------------------------
//

int getFileSize(FILE* fileName, char* file_name) // stat
{
    assert(fileName != NULL);

    struct stat fileStat;
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

//
//-------------------------
// find count of strings if input file
//-------------------------
//

int getLinesCount(char* buffer, int buffer_size)
{
    assert(buffer != NULL);

    int lines_count = 1;
    for (int index = 0; index < buffer_size; ++index)
    {
        if (buffer[index] == '\n')
        {
            ++lines_count;
            buffer[index] = '\0';
        }
    }
    return lines_count;
}

//
//-------------------------
// function make string (from buffer)
//-------------------------
//

void convertToStringsArray(char* buffer, int file_size, int strings_count, Line* strings_array)
{
    assert(buffer != NULL);
    assert(strings_array != NULL);

    strings_array[0].string_ = buffer;
    int i = 0;
    for (int indexOfLine = 1; indexOfLine <= strings_count; ++indexOfLine)
    {
        int count = 0;
        while (buffer[i] != '\0')
        {
            ++i;
            ++count;
        }

        if (buffer[i] == '\0')
        {
            strings_array[indexOfLine - 1].len_ = count;
            if (indexOfLine < strings_count)
            {
                strings_array[indexOfLine].string_ = &buffer[i + 1];
                ++i;
            }
            else 
            {
                break;
            }
        }
    }
}

//
//-------------------------
// reversSort
//-------------------------
//

void reverseSort(void* pointer, int strings_count)
{
    assert(pointer != NULL);
    
    Line* lines = (Line*)pointer; //type
    for (int index1 = 0; index1 < strings_count; ++index1)
    {
        for (int index2 = strings_count - 1; index2 > index1; --index2)
        {
            int first_line_letter_position = 1;
            while (!myIsAlphabet(*(lines[index2 - 1].string_ + lines[index2 - 1].len_ - first_line_letter_position)))
            {
                ++first_line_letter_position;
            }

            int second_line_letter_position = 1;
            while (!myIsAlphabet(*(lines[index2].string_ + lines[index2].len_ - second_line_letter_position)))
            {
                ++second_line_letter_position;
            }

            int position = 0;
            while (*(lines[index2 - 1].string_ + lines[index2 - 1].len_ - first_line_letter_position - position)
                == *(lines[index2].string_ + lines[index2].len_ - second_line_letter_position - position))
            {
                ++position;
            }

            if (myStrReversCmp(lines, index2, first_line_letter_position, second_line_letter_position, 
                position, lines[index2 - 1].len_, lines[index2].len_))
            {
                mySwap(lines, index2);
            }
        }
    }
}

//
//-------------------------
// reversStrCmp
//-------------------------
//

bool myStrReversCmp(void* pointer, int index, int first_line_letter_position, int second_line_letter_position, int position, 
                    int len1, int len2)
{
    assert(pointer != NULL);

    Line* lines = (Line*)pointer; //type

    return (*(lines[index - 1].string_ + len1 - first_line_letter_position - position)
    > *(lines[index].string_ + len2 - second_line_letter_position - position));
}

//
//-------------------------
// strings sortings
//-------------------------
//

void sortArray(void* pointer, int strings_count)
{
    assert(pointer != NULL);

    Line* lines = (Line*)pointer; //type
    for (int index1 = 0; index1 < strings_count; ++index1)
    {
        for (int index2 = strings_count - 1; index2 > index1; --index2)
        {
            int first_line_letter_position = 0;
            while (!myIsAlphabet(*(lines[index2 - 1].string_ + first_line_letter_position)))
            {
                ++first_line_letter_position;
            }

            int second_line_letter_position = 0;
            while (!myIsAlphabet(*(lines[index2].string_ + second_line_letter_position)))
            {
                ++second_line_letter_position;
            }

            int position = 0;
            while (*(lines[index2 - 1].string_ + first_line_letter_position + position)
                   == *(lines[index2].string_ + second_line_letter_position + position))
            {
                ++position;
            }

            if (myStrCmp(lines, index2, first_line_letter_position, second_line_letter_position, position))
            {
                mySwap(lines, index2);
            }
        }
    }
}

//
//-------------------------
//string compare
//-------------------------
//

bool myStrCmp(void* pointer, int index, int first_line_letter_position, int second_line_letter_position, int position)
{
    assert(pointer != NULL);

    Line* lines = (Line*)pointer; //type

    return (*(lines[index - 1].string_ + first_line_letter_position + position) 
            > *(lines[index].string_ + second_line_letter_position + position));
}

//
//-------------------------
//check symbol to alphabet
//-------------------------
//

bool myIsAlphabet(char symbol)
{
    return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

//
//-------------------------
//change strings
//-------------------------
//

void mySwap(void* pointer, int index)
{
    assert(pointer != NULL);

    Line* lines = (Line*)pointer; //type
    Line temp = lines[index - 1];
    lines[index - 1] = lines[index];
    lines[index] = temp;
}

//
//-------------------------
//write sort output file in txt
//-------------------------
//

void saveToTxtFile(Line* strings_array, int strings_count)
{
    assert(strings_array != NULL);

    FILE* fileOut = fopen("sortHamlet.txt", "w");

    if (fileOut == NULL)
    {
        printf("WRITING ERROR");
    }

    for (int index = 0; index < strings_count; ++index)
    {
        fprintf(fileOut, "%s\n", strings_array[index].string_);
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
    //getFileSize
    int count1 = NULL;
    int count2 = NULL;

    FILE* test1 = fopen("findFileSizeTest1.txt", "r");
    FILE* test2 = fopen("findFileSizeTest2.txt", "r");


    char file1[] = "findFileSizeTest1.txt";
    char file2[] = "findFileSizeTest2.txt";
    count1 = getFileSize(test1, file1);
    count2 = getFileSize(test2, file2);

    int realCount1 = 34;
    int realCount2 = 35;

    bool value1 = (realCount1 == count1);
    bool value2 = (realCount2 == count2);
    printTestResult(value1, __LINE__);
    printTestResult(value2, __LINE__);

    //getLinesCount
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

    count1 = getLinesCount(text1, size1);
    count2 = getLinesCount(text2, size2);

    realCount1 = 4;
    realCount2 = 4;

    value1 = (count1 == realCount1);
    value2 = (count2 == realCount2);
    printTestResult(value1, __LINE__);
    printTestResult(value2, __LINE__);

    free(text1);
    free(text2);
}

//
//-------------------------
// check correct or incorrect unittest
//-------------------------
//

void printTestResult(bool value, int line)
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