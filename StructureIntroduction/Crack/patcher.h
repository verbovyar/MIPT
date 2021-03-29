#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

const int SCREEN_WIDTH 	= 1280;
const int SCREEN_HEIGHT = 720;
const int FILE_HESH     = 0x9ce2f51e;

struct Text
{
    char* buffer     = NULL;
    size_t text_size = 0;
};

struct App
{
    SDL_Renderer *renderer = NULL;
    SDL_Window   *window   = NULL;
    SDL_Surface  *error    = NULL;
    SDL_Texture  *texture  = NULL;
};

size_t GetFileSize   (const char* file_name);
Text ReadFileToBuffer(const char* file_name, const char* read_type);

void Patch           (Text* text);
void Write           (Text* text);

void InitSDL         (App* app);
void DoInput         (App* app);
void PrepareScene    (App* app);
void PresentScene    (App* app);
void DrawRect        (App* app, int perc);

SDL_Texture* RenderText(const std::string &message, const std::string &font_file,
        SDL_Color color, int font_size, SDL_Renderer *renderer);
void RenderTexture  (SDL_Texture *text, SDL_Renderer *renderer, int x, int y);
void DrawText       (App* app);

bool CheckFile      (Text* check, Text* text);

