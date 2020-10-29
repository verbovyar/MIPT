#define _CRT_SECURE_NO_WARNINGS

#include "assembler.h"
#include <string.h>

const int MAX_SIZE = 1e6;
const int MAX_LABLE_SIZE = 1e4;
const char* DELIMETER = " ;\t";

int PC = 0;
int lab_ind = 0;
const char* temp;
char temp_token[256];
comandsList comands = {};
char name[] = "comand.txt";
void interpritation()
{
    readcomands(&comands, name);

    linesConstruct(&comands);

    Label* label = labelConstr();

    char* code_buffer = (char*)calloc(MAX_SIZE, sizeof(char));
    
    for (int k = 0; k < 2; ++k)
    {
        for (int i = 0; i < comands.comands_count; ++i)
        {
            strcpy(temp_token, comands.lines[i]);
            const char* token = strtok(temp_token, DELIMETER);

            #define DEF_CMD(name, num, arg, code)                           \
                                                                            \
            if (strcmp(token, #name) == 0)                                  \
            {                                                               \
                code_buffer[PC] = num;                                      \
                if (arg > 0)                                                \
                {                                                           \
                    token = strtok(NULL, DELIMETER);                        \
                    addNumber(code_buffer, token, label);                   \
                }                                                           \
                ++PC;                                                       \
            }                                                               \
            else 
            #include "comands.h"
            if (isLabel(token = strtok(NULL, DELIMETER)))
            {
                writeLableName(label);
            }
            else
            {
                printf("ERROR");
            }

            #undef DEF_CMD
        }
    }
    
    writeToBin(code_buffer, PC);

    free(code_buffer);
}

void writeLableName(Label* label)
{
    label[lab_ind].lab_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
    strcpy(label[lab_ind].lab_name, temp);
    label[lab_ind].PC = PC + 1;
    ++lab_ind;
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

Label* labelConstr()
{
    Label* label = (Label*)calloc(MAX_LABLE_SIZE, sizeof(Label));

    for (int i = 0; i < MAX_LABLE_SIZE; ++i)
    {
        label[i].PC = -1;
    }

    return label;
}

bool isLabel(const char* token)
{
    const char* elem[] = { ":" };
    if (strcmp(token, elem[0]) == 0)
    {
        return true;
    }

    return false;
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

void addNumber(char* code_buffer, const char* token, Label* label)
{
    double count = 0;
    const char* registers[] = { "rax", "rbx", "rcx", "rdx" };

    if (sscanf(token, "%lf", &count))                       
    {                                                       
        code_buffer[++PC] = 0;                               
        ++PC;                                                
        *(double*)(code_buffer + PC) = count;                
        PC += sizeof(double) - 1;                            
    }    
    else if (myIsAlphabet(*token))
    {
        int i = 0;
        while (strcmp(token, label[i].lab_name) != 0)
        {
            ++i;
        }
        code_buffer[PC + 1] = label[i].PC;
        ++PC;
    }
    else                                                    
    {                                                       
        for (int index = 0; index < 4; ++index)             
        {                                                   
            if (strcmp(token, registers[index]) == 0)       
            {                                               
                code_buffer[++PC] = index + 1;               
            }                                               
        }                                                   
    }
}

bool myIsAlphabet(const char symbol)
{
    return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

void writeToBin(char* code_buffer, int PC)
{
    FILE* file = fopen("assembler_code.bin", "wb");

    fwrite(code_buffer, sizeof(char), PC, file);

    fclose(file);
}