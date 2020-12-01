#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

const int MAX_SIZE = 1e8;

struct Text {
    char* buffer = nullptr;
    size_t text_size = 0;
    size_t ofs = 0;
};

//----

Text* newText();
void textConstruct   (Text* text);
size_t getFileSize   (const char* file_name);
Text readFileToBuffer(const char* file_name);

//----

double getAnswer    (Text* text);
double getExpression(Text* text);
double getOperation (Text* text);
double getBracket   (Text* text);
double getNumber    (Text* text);

//----

int main()
{
    Text* text = newText();
    double answer = getAnswer(text);

    printf("Answer:%lf\n\n", answer);

    return 0;
}

//----
Text* newText()
{
    Text* text = (Text*)calloc(1, sizeof(Text));
    assert(text != nullptr);
    text->text_size = 0;
    text->ofs = 0;

    textConstruct(text);

    return text;
}

void textConstruct(Text* text)
{
    assert(text != nullptr);

    getFileSize("function.txt");
    *text = readFileToBuffer("function.txt");
}

size_t getFileSize(const char* file_name)
{
    assert(file_name != nullptr);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

Text readFileToBuffer(const char* file_name)
{
    assert(file_name != nullptr);

    FILE* file_descriptor = fopen("function.txt", "r");

    size_t file_size = getFileSize(file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    assert(buffer != nullptr);
    file_size = fread(buffer, sizeof(char), file_size + 1, file_descriptor);

    fclose(file_descriptor);

    Text text_params = { buffer, file_size };

    return text_params;
}

//---- Calc

double getAnswer(Text* text)
{
    double answer = getExpression(text);
    if (text->buffer[text->ofs] != '\0')
    {
        printf("Syntax error!\n");
        return -1;
    }

    return answer;
}
double getExpression(Text* text)
{
    double value1 = getOperation(text);
    while (text->buffer[text->ofs] == '+' || text->buffer[text->ofs] == '-')
    {
        char operation = text->buffer[text->ofs];
        ++text->ofs;
        double value2 = getOperation(text);
        if (operation == '+')
        {
            value1 += value2;
        }
        else
        {
            value1 -= value2;
        }
    }

    return value1;
}

double getOperation(Text* text)
{
    double value1 = getBracket(text);
    while (text->buffer[text->ofs] == '*' || text->buffer[text->ofs] == '/')
    {
        char operation = text->buffer[text->ofs];
        ++text->ofs;
        double value2 = getBracket(text);
        if (operation == '*')
        {
            value1 *= value2;
        }
        else
        {
            value1 /= value2;
        }
    }

    return value1;
}

double getBracket(Text* text)
{
    double answer = 0;
    if (text->buffer[text->ofs] == '(')
    {
        ++text->ofs;
        answer = getExpression(text);
        if (text->buffer[text->ofs] == ')')
        {
            ++text->ofs;
        }
        else
        {
            printf("bracket error!");
        }
    }
    else
    {
        answer = getNumber(text);
    }

    return answer;
}

double getNumber(Text* text)
{
    int size = 0;
    double value = 0;
    sscanf(&text->buffer[text->ofs], "%lf%n", &value, &size);

    text->ofs += size;

    return value;
}
