#define _CRT_SECURE_NO_WARNINGS

#include "assembler.h"

const int MAX_SIZE = 1e6;
const int MAX_LABLE_SIZE = 1e4;
const int MAX_LABLE_SYMBOLS = 15;
const char* DELIMETER = " ;\t";

comandsList comands = {};
void interpritation()
{
    char name[] = "comand.txt";
    readcomands(&comands, name);

    linesConstruct(&comands);

    Label* label = labelConstr();

    char* code_buffer = (char*)calloc(MAX_SIZE, sizeof(char));
    const char* temp; //!!!
    for (int k = 0; k < 2; ++k)
    {
        comands.OFS = 0;
        int lab_ind = 0;
        for (int i = 0; i < comands.comands_count; ++i)
        {
            char temp_token[256];
            strcpy(temp_token, comands.lines[i]);
            const char* token = strtok(temp_token, DELIMETER);
            temp = token;

            #define DEF_CMD(name, num, arg, code)                               \
                                                                                \
                if (strcmp(token, #name) == 0)                                  \
                {                                                               \
                    code_buffer[comands.OFS] = num;                             \
                    if (arg > 0)                                                \
                    {                                                           \
                        token = strtok(NULL, DELIMETER);                        \
                        addArgument(code_buffer, token, label, lab_ind);        \
                    }                                                           \
                    ++comands.OFS;                                              \
                }                                                               \
                else 
            #include "../../comands.h"
            if (isLabel(token = strtok(NULL, DELIMETER)))
            {
                writeLableName(label, temp, &lab_ind);
            }
            else
            {
                printf("ERROR");
            }

            #undef DEF_CMD
        }
    }

    for (int i = 0; i < 8; ++i)
    {
       // printf("[%d] %s\n", label[i].PC, label[i].lab_name);
    }

    writeToBin(code_buffer, comands.OFS);

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

Label* labelConstr()
{
    Label* label = (Label*)calloc(MAX_LABLE_SIZE, sizeof(Label));
    return label;
}

bool isLabel(const char* token)
{
    const char* elem[] = { ":" };
    return (strcmp(token, elem[0]) == 0);
}

void writeLableName(Label* label, const char* temp, int* lab_ind)
{
    label[*lab_ind].lab_name = (char*)calloc(strlen(temp) + 1, sizeof(char));
    strcpy(label[*lab_ind].lab_name, temp);
    label[*lab_ind].PC = comands.OFS + 1;
    //printf("[%d] %s\n", label[*lab_ind].PC, label[*lab_ind].lab_name);
    ++(*lab_ind);
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

const char* registers[] = { "rax", "rbx", "rcx", "rdx" };
void addArgument(char* code_buffer, const char* token, Label* label, int lab_ind)
{
    double numb = 0;

    if (sscanf(token, "%lf", &numb))
    {
        ++comands.OFS;
        code_buffer[comands.OFS] = 0;
        *(double*)(code_buffer + comands.OFS + 1) = numb;
        comands.OFS += sizeof(double);
    }
    else if (myIsAlphabet(*token))
    {
        int i = 0;
        while (i < MAX_LABLE_SIZE && label[i].lab_name != NULL && strcmp(token, label[i].lab_name) != 0)
        {
            ++i;
        }
        //printf("[%d] %s\n", label[i].PC, label[i].lab_name);
        *(int*)(code_buffer + comands.OFS + 1) = label[i].PC;
        comands.OFS += sizeof(int);
    }
    else
    {
        for (int index = 0; index < 4; ++index)
        {
            if (strcmp(token, registers[index]) == 0)
            {
                ++comands.OFS;
                code_buffer[comands.OFS] = index + 1;
            }
        }
    }
}

bool myIsAlphabet(const char symbol)
{
    return (symbol >= 'A' && symbol <= 'Z');
}

void writeToBin(char* code_buffer, int PC)
{
    FILE* file = fopen("assembler_code.bin", "wb");

    fwrite(code_buffer, sizeof(char), PC, file);

    fclose(file);
}

