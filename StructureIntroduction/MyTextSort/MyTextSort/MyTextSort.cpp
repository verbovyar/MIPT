#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX = 1000;
const int MAXLEN = 4000;

void separationOnLines(char* line[], char* templine, int countOfStr, int index)
{
    int j = 0;
    for (int i = 0; i < countOfStr; ++i)
    {
        char* temp = (char*)calloc(MAX, sizeof(char));
        int k = 0;
        for (; j < index; ++j)
        {
            if (templine[j] != '\n')
            {
                temp[k] = templine[j];
                k++;
            }
            else
            {
                ++j;
                break;
            }
        }

        line[i] = temp;
    }
}

void sortLines(char* line[], int countOfStr)
{
    for (int index1 = 0; index1 < countOfStr; ++index1)
    {
        for (int index2 = countOfStr - 1; index2 > index1; index2--)
        {
            int i = 0;
            while (*(line[index2 - 1] + i) == *(line[index2] + i))
            {
                ++i;
            }

            if (*(line[index2 - 1] + i) > *(line[index2] + i))
            {
                char *temp = line[index2 - 1];
                line[index2 - 1] = line[index2];
                line[index2] = temp;
            }
        }
    }
}

void writeTxtFile(char* line[], int countOfStr)
{
    FILE* fileout = fopen("output.txt", "w");

    for (int k = 0; k < countOfStr; ++k)
    {
        fprintf(fileout, "%s\n", line[k]);
    }

    fclose(fileout);
}

void readTxtFileInOneLine(char* line[])
{
    FILE* hamlet = fopen("hamlet.txt", "r");

    if (hamlet == NULL)
    {
        printf("ERROR!");

        return;
    }

    char templine[MAXLEN] = { 0 };
    char symb;
    int countOfStr = 0;
    int index = 0;
    while ((symb = fgetc(hamlet)) != EOF)
    {
        templine[index] = symb;
        ++index;

        if (symb == '\n')
        {
            ++countOfStr;
        }
    }

    fclose(hamlet);

    // in main;
    separationOnLines(line, templine, countOfStr, index);

    sortLines(line, countOfStr);

    writeTxtFile(line, countOfStr);
}

int main()
{
    char* hamletLines[36];

    readTxtFileInOneLine(hamletLines);

    return 0;
}
