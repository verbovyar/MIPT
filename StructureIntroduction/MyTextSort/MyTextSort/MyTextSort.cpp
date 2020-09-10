#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX = 1000;
const int MAXLEN = 4000;

void writeTxtFile(char* line[], int countOfStr)
{
    FILE* fileout = fopen("output.txt", "w");

    for (int index = 0; index < countOfStr; ++index)
    {
        fprintf(fileout, "%s\n", line[index]);
    }

    fclose(fileout);
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

            if (*(line[index2 - 1] + i) > * (line[index2] + i))
            {
                char* temp = line[index2 - 1];
                line[index2 - 1] = line[index2];
                line[index2] = temp;
            }
        }
    }

    writeTxtFile(line, countOfStr);
}

void separationOnLines(char* line[], char* templine, int countOfStr, int count)
{
    int index1 = 0;
    for (int index = 0; index < countOfStr; ++index)
    {
        char* temp = (char*)calloc(MAX, sizeof(char));
        int index2 = 0;
        for (; index1 < count; ++index1)
        {
            if (templine[index1] != '\n')
            {
                temp[index2] = templine[index1];
                index2++;
            }
            else
            {
                ++index1;
                break;
            }
        }

        line[index] = temp;
    }

    sortLines(line, countOfStr);
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

    separationOnLines(line, templine, countOfStr, index);
}

int main()
{
    char* hamletLines[36];

    readTxtFileInOneLine(hamletLines);

    printf("successful\n");

    return 0;
}
