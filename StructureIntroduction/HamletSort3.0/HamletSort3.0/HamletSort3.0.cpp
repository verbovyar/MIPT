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
    size_t len_ = 0;
};

struct Text {
    char* buffer_ = NULL;
    size_t text_size_ = 0;
    size_t lines_count = 0;
    Line* lines_ = NULL;
};

enum ERROR_CODES {
    NO_ERROR = 0,
    READING_ERROR = 101
};

ERROR_CODES error_status = NO_ERROR;

//
//-------------------------
//functions prototypes
//-------------------------
//

Text readFileToBuffer(const char* file_name);
size_t getFileSize(const char* file_name);
size_t getLinesCount(char* buffer, size_t buffer_size);
void textConstruct(Text* text); 
bool myStrReversCmp(void* str1, void* str2);
bool myStrCmp(void* str1, void* str2);
void myQsort(Line* pointer, int size, bool(*cmp)(void*, void*));
bool myIsAlphabet(char symbol);
void mySwap(Line* str1, Line* str2);
void sortHamletTxt(const char* file_name, Text text);
void writeToTxtFile(Line* strings_array, size_t strings_count, FILE* fileout);
void textDestruct(Text* text);
void runUnitTests();
void printTestResult(bool value, size_t line);

//
//-------------------------
// main (add buffer and read file in one buffer)
//-------------------------
//

int main()
{
    runUnitTests ();

    Text text = readFileToBuffer("hamlet.txt");
    if (error_status == READING_ERROR) return READING_ERROR;

    textConstruct(&text);
        
    sortHamletTxt("sortHamlet.txt", text);

    textDestruct(&text);
   
    printf("SUCCESSFUL ENDING");

    return 0;
}

//
//-------------------------
//read file into buffer
//-------------------------
//

Text readFileToBuffer(const char* file_name)
{
    assert(file_name != NULL);

    FILE* file_descriptor = fopen("hamlet.txt", "r");

    if(file_descriptor == NULL)
    {
        printf("\nREADING ERROR!");
        error_status = READING_ERROR;
    }
    
    size_t file_size = getFileSize(file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    assert(buffer != NULL);
    file_size = fread(buffer, sizeof(char), file_size + 1, file_descriptor);

    fclose(file_descriptor);

    Text text_params = {buffer, file_size};

    return text_params;
}

//
//-------------------------
// function find size of input file
//-------------------------
//

size_t getFileSize(const char* file_name) 
{
    assert(file_name != NULL);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

//
//-------------------------
// find count of strings if input file
//-------------------------
//

size_t getLinesCount(char* buffer, size_t buffer_size)
{
    assert(buffer != NULL);

    size_t lines_count = 1;
    for (size_t index = 0; index < buffer_size; ++index)
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

void textConstruct(Text* text)
{
    assert(text != NULL);

    size_t lines_count = getLinesCount(text->buffer_, text->text_size_);
    text->lines_ = (Line*)calloc(lines_count, sizeof(Text));
    assert(text->lines_ != NULL);
    text->lines_count = lines_count;
    text->lines_[0].string_ = text->buffer_;
    size_t i = 0;
    size_t count = 0;
    for (size_t indexOfLine = 1; indexOfLine <= lines_count; ++indexOfLine)
    {
        count = 0;
        while (text->buffer_[i] != '\0')
        {
            ++i;
            ++count;
        }

        if (text->buffer_[i] == '\0')
        {
            text->lines_[indexOfLine - 1].len_ = count;
            if (indexOfLine < lines_count)
            {
                text->lines_[indexOfLine].string_ = &text->buffer_[i + 1];
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
// reversStrCmp
//-------------------------
//

bool myStrReversCmp(void* str1, void* str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    Line* line1 = (Line*)str1;
    Line* line2 = (Line*)str2;

    size_t first_line_letter_position = 0;
    while (!myIsAlphabet(*(line1->string_ + line1->len_ - first_line_letter_position)))
    {
        ++first_line_letter_position;
    }

    size_t second_line_letter_position = 0;
    while (!myIsAlphabet(*(line2->string_ + line2->len_ - second_line_letter_position)))
    {
        ++second_line_letter_position;
    }

    size_t position = 0;
    while (*(line1->string_ + line1->len_ - first_line_letter_position - position)
        == *(line2->string_ + line2->len_ - second_line_letter_position - position))
    {
        ++position;
    }

    return (*(line1->string_ + line1->len_ - first_line_letter_position - position)
    > *(line2->string_ + line2->len_ - second_line_letter_position - position));
}

//
//-------------------------
// strings sortings
//-------------------------
//

int partition(void* pointer, int size, bool(*cmp)(void*, void*))
{
    assert(pointer != NULL);
    Line* lines = (Line*)pointer;
    int i = 1;

    for (int j = 1; j < size; ++j)
    {
        if (cmp(lines, lines + j))
        {
            mySwap(lines + i, lines + j);
            ++i;
        }
    }
    mySwap(lines + i - 1, lines);

    return i - 1;
}

void myQsort(Line* pointer, int size, bool(*cmp)(void*, void*))
{
    assert(pointer != NULL);

    if (size <= 1)
    {
        return;
    }
    if (size == 2)
    {
        if (cmp(pointer, pointer + 1))
        {
            mySwap(pointer, pointer + 1);
        }
        else
        {
            return;
        }
    }
    int position = partition(pointer, size, cmp);

    myQsort(pointer, position, cmp);
    myQsort(pointer + (position + 1), size - position - 1, cmp);
}

//
//-------------------------
//string compare
//-------------------------
//

bool myStrCmp(void* str1, void* str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    Line* line1 = (Line*)str1;
    Line* line2 = (Line*)str2;

    size_t first_line_letter_position = 0;
    while (!myIsAlphabet(*(line1->string_ + first_line_letter_position)))
    {
        ++first_line_letter_position;
    }

    size_t second_line_letter_position = 0;
    while (!myIsAlphabet(*(line2->string_ + second_line_letter_position)))
    {
        ++second_line_letter_position;
    }

    size_t position = 0;
    while (*(line1->string_ + first_line_letter_position + position)
        == *(line2->string_ + second_line_letter_position + position) 
             && position < line1->len_ && position < line2->len_)
    {
        ++position;
    }

    return (*(line1->string_ + first_line_letter_position + position) 
            > *(line2->string_ + second_line_letter_position + position));
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

void mySwap(Line* str1, Line* str2)
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    Line temp = *str1;
    *str1 = *str2;
    *str2 = temp; 
}

//
//-------------------------
//write sort output file in txt
//-------------------------
//

void sortHamletTxt(const char* file_name, Text text)
{
    FILE* file_discriptor = fopen(file_name, "w");

    myQsort(text.lines_, text.lines_count, myStrCmp);
    writeToTxtFile(text.lines_, text.lines_count, file_discriptor);

    myQsort(text.lines_, text.lines_count, myStrReversCmp);
    writeToTxtFile(text.lines_, text.lines_count, file_discriptor);

    fclose(file_discriptor);
}

void writeToTxtFile(Line* strings_array, size_t strings_count, FILE* fileout)
{
    assert(strings_array != NULL);

    FILE* fileOut = fopen("sortHamlet.txt", "a");

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
//destroy file
//-------------------------
//

void textDestruct(Text* text)
{
    assert(text != NULL);
    assert(text->buffer_ != NULL);
    assert(text->lines_ != NULL);

    free(text->buffer_);
    free(text->lines_);
    text->lines_count = 0;
    text->text_size_ = 0;
}

//
//-------------------------
//unit testing
//-------------------------
//

void runUnitTests()
{
    //getFileSize
    size_t count1 = NULL;
    size_t count2 = NULL;

    FILE* test1 = fopen("findFileSizeTest1.txt", "r");
    FILE* test2 = fopen("findFileSizeTest2.txt", "r");


    char file1[] = "findFileSizeTest1.txt";
    char file2[] = "findFileSizeTest2.txt";
    count1 = getFileSize(file1);
    count2 = getFileSize(file2);

    size_t realCount1 = 34;
    size_t realCount2 = 35;

    bool value1 = (realCount1 == count1);
    bool value2 = (realCount2 == count2);
    printTestResult(value1, __LINE__);
    printTestResult(value2, __LINE__);

    //getLinesCount
    test1 = fopen("findCountOfStrTest1.txt", "r");
    test2 = fopen("findCountOfStrTest2.txt", "r");

    size_t size1 = 34;
    size_t size2 = 21;
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

void printTestResult(bool value, size_t line)
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