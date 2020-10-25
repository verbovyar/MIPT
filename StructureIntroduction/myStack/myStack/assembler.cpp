#define _CRT_SECURE_NO_WARNINGS

#include "assembler.h"
#include <string.h>

const int MAX_SIZE = 1e6;
const int MAX_LABLE_SIZE = 1e4;
const char* DELIMETER = " ;\t";

comandsList comands = {};
void interpritation()
{
    char name[] = "comand.txt";

    readcomands(&comands, name);

    linesConstruct(&comands);

    char* code_buffer = (char*)calloc(MAX_SIZE, sizeof(char));
    int j = 0;
    char temp;
    double count = 0;
    const char* registers[] = { "rax", "rbx", "rcx", "rdx" };
    for (int i = 0; i < comands.comands_count; ++i)
    {
        const char* token = strtok(comands.lines[i], DELIMETER);
        #define DEF_CMD(name, num, arg, code)                           \
        if (strcmp(token, #name) == 0)                                  \
        {                                                               \
            code_buffer[j] = num;                                       \
            if (arg > 0)                                                \
            {                                                           \
                token = strtok(NULL, DELIMETER);                        \
                if (sscanf(token, "%lf", &count))                       \
                {                                                       \
                    code_buffer[++j] = 0;                               \
                    ++j;                                                \
                    *(double*)(code_buffer + j) = count;                \
                    j += sizeof(double);                                \
                    j--;                                                \
                }                                                       \
                else                                                    \
                {                                                       \
                    for (int index = 0; index < 4; ++index)             \
                    {                                                   \
                        if (strcmp(token, registers[index]) == 0)       \
                        {                                               \
                            code_buffer[++j] = index + 1;               \
                        }                                               \
                    }                                                   \
                }                                                       \
            }                                                           \
            ++j;                                                        \
        }                                                               \
        else   
        #include "comands.h"                                                 
        {
            printf("ERROR");
        }
        #undef DEF_CMD
    }

    writeToBin(code_buffer, j);

    free(code_buffer);
}

READERROR readcomands(comandsList* comands, char* name)
{
    assert(comands != NULL);

    FILE* file_descriptor = fopen(name, "r");

    if (file_descriptor == NULL)
    {
        printf("\nREADING ERROR!");

        return READ_ERROR;
    }

    comands->buffer_size = getFileSize("comand.txt");
    comands->buffer = (char*)calloc(comands->buffer_size + 1, sizeof(char));
    fread(comands->buffer, sizeof(char), comands->buffer_size + 1, file_descriptor);

    fclose(file_descriptor);

    return NON_READ_ERROR;
}

size_t getFileSize(const char* file_name)
{
    assert(file_name != NULL);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

size_t getLinesCount(comandsList* comands)
{
    assert(comands != NULL);

    size_t lines_count = 1;
    for (size_t index = 0; index < comands->buffer_size; ++index)
    {
        if (comands->buffer[index] == '\n')
        {
            ++lines_count;
            comands->buffer[index] = '\0';
        }
    }

    return lines_count;
}

void linesConstruct(comandsList* comands)
{
    assert(comands != NULL);

    comands->comands_count = getLinesCount(comands);
    comands->lines = (char**)calloc(comands->comands_count, sizeof(char*));
    comands->lines[0] = comands->buffer;

    size_t i = 0;
    for (size_t indexOfLine = 1; indexOfLine <= comands->comands_count; ++indexOfLine)
    {
        while (comands->buffer[i] != '\0')
        {
            ++i;
        }

        if (comands->buffer[i] == '\0')
        {
            if (indexOfLine < comands->comands_count)
            {
                comands->lines[indexOfLine] = &comands->buffer[i + 1];
                ++i;
            }
            else
            {
                break;
            }
        }
    }
}

void writeToBin(char* code_buffer, int PC)
{
    FILE* file = fopen("assembler_code.bin", "wb");

    fwrite(code_buffer, sizeof(char), PC, file);

    fclose(file);
}
