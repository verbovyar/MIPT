#define _CRT_SECURE_NO_WARNINGS

#include "interpritation.h"

comandsList comands = {};
void interpritation()
{    
    char name[] = "comand.txt";
    if (readcomands(&comands, name) == READ_ERROR)
    {
        return;
    }

    linesConstruct(&comands);

    FILE* file_descriptor = fopen("assembler_code.txt", "w");

    const char* array_comands[] = { "push", "summ", "min", "del", "mult","pow", "sqrt", "sin", "cos", "hult", "out" };
    for (int j = 0; j < comands.comands_count; ++j)
    {
        if (j < comands.comands_count - 1 && !isalpha(*(comands.lines[j + 1] - 2)))
        {
            fprintf(file_descriptor, "%s" , "0");
            fprintf(file_descriptor, "%s", comands.lines[j + 1] - 2);// > 9
        }

        for (int i = 0; i < 11; ++i)
        {
            if (strcmp(array_comands[i], comands.lines[j]) == 0)
            {
                char* ptr = (char*)calloc(1, sizeof(char));
                _itoa(i, ptr, 11);
                if (ptr == NULL)
                {
                    return;
                }
                fprintf(file_descriptor, "%s", ptr);
            }
        }
    }

    fclose(file_descriptor);
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
