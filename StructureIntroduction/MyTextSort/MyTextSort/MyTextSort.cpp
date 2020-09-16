#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// new funct
int findFileSize(FILE* fileName)
{
    fseek(fileName, 0, SEEK_END);
    const int sizeOfFile = ftell(fileName);
    fseek(fileName, 0, SEEK_SET);

    return sizeOfFile;
}

int findCountOfStr(char* buffer, int sizeOfFile)
{
    int countOfStr = 0;

    for (int index = 0; index < sizeOfFile; ++index)
    {
        if (buffer[index] == '\n')
        {
            ++countOfStr;
        }
    }

    return countOfStr;
}

//void makeLines(char* buffer, char** lines, int sizeOfFile)
//{
 //   for (int index = 0; index < sizeOfFile; ++index)
 //   {

 //   }
//} 

int main()
{
    FILE* fileName = fopen("hamlet.txt", "r"); // add const value for TXT file 

    const int sizeOfFile = findFileSize(fileName);
    char* buffer = (char*)calloc(sizeOfFile + 1, sizeof(char));
    fread(buffer, sizeof(char), sizeOfFile, fileName);
    
    fclose(fileName);

    const int countOfStr = findCountOfStr(buffer, sizeOfFile);
    char** lines = (char**)calloc(countOfStr + 1, sizeof(char*));
   // makeLines(buffer, lines, sizeOfFile);

    return 0;
}
