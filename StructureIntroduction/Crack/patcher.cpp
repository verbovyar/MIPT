#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

struct Text {
    char* buffer = NULL;
    size_t text_size = 0;
};

size_t GetFileSize(const char* file_name) 
{
    assert(file_name != NULL);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

Text ReadFileToBuffer(const char* file_name)
{
    assert(file_name != NULL);

    FILE* file_descriptor = fopen(file_name, "rb");

    size_t file_size = GetFileSize(file_name);
    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    assert(buffer != NULL);
    file_size = fread(buffer, sizeof(char), file_size + 1, file_descriptor);

    fclose(file_descriptor);

    Text text_params = {buffer, file_size};

    return text_params;
}

void Patch(Text* text)
{
    text->buffer[0x1061] = 0x48;
    text->buffer[0x1062] = 0xc7;
    text->buffer[0x1063] = 0xc1;
    text->buffer[0x1064] = 0x00;
    text->buffer[0x1065] = 0x00;
    text->buffer[0x1066] = 0x00;
    text->buffer[0x1067] = 0x00;
    text->buffer[0x1068] = 0x90;
}

void Write(Text* text)
{
    FILE* file_discriptor = fopen("hack_sosiliy.out", "wb");

    fwrite(text->buffer, 1, text->text_size, file_discriptor);

    fclose(file_discriptor);
    free(text->buffer);
}

int main()
{
    Text text = ReadFileToBuffer("pass.out");

    Patch(&text);

    Write(&text);

    return 0;
}