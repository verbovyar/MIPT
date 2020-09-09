#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX = 1000;
const int MAXLEN = 50000;

void readTxtFile(char* line[])
{
    FILE* hamlet = fopen("hamlet.txt", "r");

    char templine[MAXLEN] = { 0 };

    if (hamlet == NULL)
    {
        printf("ERROR!");

        return;
    }

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

    FILE* fileout = fopen("output.txt", "w");

    for (int k = 0; k < countOfStr; ++k)
    {
        fprintf(fileout, "%s\n", line[k]);
    }

    fclose(fileout);
}

int main()
{
    char* hamletLines[36];
    readTxtFile(hamletLines);

    return 0;
}
