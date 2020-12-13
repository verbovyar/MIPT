#define _CRT_SECURE_NO_WARNINGS

#include "assembler.h"

const char* DELIMETER = " ;\t";
const int RAM_MODE = 5;
const int SUM = 404;
const int DEC = 505;

void getByteCode(const char* name)
{
    comandsList* comands = comandsConstr();
    readcomands(comands, name);
    linesConstruct(comands);

    Label* label = labelArrayConstr();

    char* code_buffer = (char*)calloc(MAX_BUFFER_SIZE, sizeof(char));
    for (int passage = 0; passage < PASEGES_COUNT; ++passage)
    {
        comands->ofs = 0;
        int lab_idx = 0;
        for (int i = 0; i < comands->comands_count; ++i)
        {
            char temp_comand[MAX_STR_SIZE];
            strcpy(temp_comand, comands->lines[i]);
            const char* token = strtok(temp_comand, DELIMETER);
            const char* temp_token = token;

            #define DEF_CMD(name, num, arg, code)                                       \
                                                                                        \
                if (strcmp(token, #name) == 0)                                          \
                {                                                                       \
                    code_buffer[comands->ofs] = num;                                    \
                    if (arg > 0)                                                        \
                    {                                                                   \
                        token = strtok(NULL, DELIMETER);                                \
                        addArgument(token, label, lab_idx, code_buffer, comands);       \
                    }                                                                   \
                    ++comands->ofs;                                                     \
                }                                                                       \
                else 
                #include "../../comands.h"
                if (isLabel(token = strtok(NULL, DELIMETER)))
                {
                    writeLableName(temp_token, label, &lab_idx, comands);
                }
                else
                {
                    printf("ERROR");
                }

            #undef DEF_CMD
        }
    }

    writeToBin(code_buffer, comands->ofs);

    free(code_buffer);
    free(label);
}

READERROR readcomands(comandsList* comands, const char* name)
{
    assert(comands != NULL);

    FILE* file_descriptor = fopen(name, "r");

    if (file_descriptor == NULL)
    {
        printf("\nREADING ERROR!");

        return READ_ERROR;
    }

    comands->buffer_size = getFileSize(name);
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

Label* labelArrayConstr()
{
    Label* label = (Label*)calloc(MAX_LABLE_ARRAY_SIZE, sizeof(Label));

    return label;
}

comandsList* comandsConstr()
{
    comandsList* comands = (comandsList*)calloc(1, sizeof(comandsList));

    return comands;
}

bool isLabel(const char* token)
{
    const char* elem[] = { ":" };

    return (strcmp(token, elem[0]) == 0);
}

void writeLableName(const char* token, Label* label, int* lab_idx, comandsList* comands)
{
    label[*lab_idx].lab_name = (char*)calloc(strlen(token) + 1, sizeof(char));
    strcpy(label[*lab_idx].lab_name, token);
    label[*lab_idx].pc = comands->ofs + 1;
    //printf("[%d] %s\n", label[*lab_idx].pc, label[*lab_idx].lab_name);
    ++(*lab_idx);
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
void addArgument(const char* token, Label* label, int lab_ind, char* code_buffer, comandsList* comands)
{
        double numb = 0;

        if (sscanf(token, "%lf", &numb))
        {
            ++comands->ofs;
            code_buffer[comands->ofs] = 0;
            *(double*)(code_buffer + comands->ofs + 1) = numb;
            comands->ofs += sizeof(double);
        }
        else if (myIsAlphabet(*token))
        {
            int i = 0;
            while (i < MAX_LABLE_ARRAY_SIZE && label[i].lab_name != NULL && strcmp(token, label[i].lab_name) != 0)
            {
                ++i;
            }
            //printf("[%d] %s\n", label[i].pc, label[i].lab_name);
            *(int*)(code_buffer + comands->ofs + 1) = label[i].pc;
            comands->ofs += sizeof(int);
        }
        else
        {
            if (token == "[")
            {
                ++comands->ofs;
                code_buffer[comands->ofs] = RAM_MODE;
            }
            else if (token == "+")
            {
                ++comands->ofs;
                code_buffer[comands->ofs] = SUM;
            }
            else if (token == "-")
            {
                ++comands->ofs;
                code_buffer[comands->ofs] = DEC;
            }
            else
            {
                for (int index = 0; index < REGISTERS_COUNT; ++index)
                {
                    if (strcmp(token, registers[index]) == 0)
                    {
                        ++comands->ofs;
                        code_buffer[comands->ofs] = index + 1;
                    }
                }
            }
        }
}

bool myIsAlphabet(const char symbol)
{
    return (symbol >= 'A' && symbol <= 'Z');
}

void writeToBin(char* code_buffer, int pc)
{
    FILE* file = fopen("assembler_code.bin", "wb");

    fwrite(code_buffer, sizeof(char), pc, file);

    fclose(file);
}

