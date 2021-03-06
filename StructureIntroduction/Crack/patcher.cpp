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

size_t GetFileSize(const char* file_name) 
{
    assert(file_name != NULL);

    struct stat fileStat = {};
    stat(file_name, &fileStat);

    return fileStat.st_size;
}

Text ReadFileToBuffer(const char* file_name, const char* read_type)
{
    assert(file_name != NULL);

    FILE* file_descriptor = fopen(file_name, read_type);

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
    assert(text != NULL);

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
    assert(text != NULL);

    FILE* file_discriptor = fopen("hack_sosiliy.out", "wb");

    fwrite(text->buffer, 1, text->text_size, file_discriptor);

    fclose(file_discriptor);
    free(text->buffer);
}

void InitSDL(App* app)
{
    assert(app != NULL);

    int renderer_flags = 0; 
    int window_flags = 0;

    renderer_flags = SDL_RENDERER_ACCELERATED;
    
    window_flags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app->window = SDL_CreateWindow("MY HACK", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    
    if (!app->window)
    {
        printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, renderer_flags);
    
    if (!app->renderer)
    {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void DoInput(App* app)
{
    assert(app != NULL);

    SDL_Event event = {};
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                exit(0);
                break;

            default:
                break;
        }
    }
}

void PrepareScene(App* app)
{
    assert(app != NULL);

    app->error = SDL_LoadBMP("Error.bmp");
    app->texture = SDL_CreateTextureFromSurface(app->renderer, app->error);

    SDL_SetRenderDrawColor(app->renderer, 96, 128, 255, 255);
    SDL_RenderClear(app->renderer);
}

void PresentScene(App* app)
{
    assert(app != NULL);

    SDL_RenderPresent(app->renderer);
}

void DrawRect(App* app, int perc)
{
    assert(app != NULL);

    int x = 490;
    int w = 300;
    int y = 500;
    int h = 50;

    SDL_SetRenderDrawColor(app->renderer, 34, 139, 34, 255);
    SDL_RenderClear(app->renderer);
    SDL_RenderCopy(app->renderer, app->texture, NULL, NULL);
    
    SDL_SetRenderDrawColor(app->renderer, 55, 55, 55, 255);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(app->renderer, &rect);

    SDL_SetRenderDrawColor(app->renderer, 0, 100, 100, 255);
    int new_x = 1.0 * w * perc / 100;
    SDL_Rect prog = {x, y, new_x, h};
    SDL_RenderFillRect(app->renderer, &prog);
}

//--------------------------------

SDL_Texture* RenderText(const std::string &message, const std::string &font_file,
        SDL_Color color, int font_size, SDL_Renderer *renderer)
{
	TTF_Font *font = TTF_OpenFont(font_file.c_str(), font_size); 
	assert(font);     

	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	assert(surf);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	return texture;
}

void RenderTexture(SDL_Texture *text, SDL_Renderer *renderer, int x, int y)
{
	SDL_Rect dst = {};
	dst.x = x;
	dst.y = y;
	dst.w = 1000;
	dst.h = 100;
	SDL_RenderCopy(renderer, text, NULL, &dst);
}

void DrawText(App* app)
{
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Texture *image = RenderText("Oops, i think file was cracked, sorry!", "SouthernAire_Personal_Use_Only.ttf",
			color, 64, app->renderer);

	int w = 110;
	int h = 110;
	SDL_QueryTexture(image, NULL, NULL, &w, &h);
	int x = SCREEN_WIDTH / 2 - w / 2;
	int y = SCREEN_HEIGHT / 2 - h / 2;	

	SDL_RenderClear(app->renderer);

	RenderTexture(image, app->renderer, 10, 10);
	SDL_RenderPresent(app->renderer);
}

bool CheckFile(Text* check, Text* text)
{
    int summ = 0;
    int i = 0;

    sscanf(check->buffer, "%x", &summ);

    return summ == FILE_HESH;
}

int main(int argc, char *argv[])
{
    Text text = ReadFileToBuffer("pass.out", "rb");

    system("rhash pass.out -o checksum.txt --simple");

    Text check = ReadFileToBuffer("checksum.txt", "r");

    if (!CheckFile(&check, &text))
    {
        printf("%s", "IT IS WRONG FILE!\n");
        return 0;
    }

    Patch(&text);

    Write(&text);

    App app = {nullptr, nullptr, nullptr, nullptr};

    memset(&app, 0, sizeof(App));
    
    InitSDL(&app);

    int perc = 0;
    while (perc != 100)
    {
        PrepareScene(&app);
        
        DoInput(&app);

        DrawRect(&app, perc);
        ++perc;

        PresentScene(&app);
        
        SDL_Delay(32);
    }

    TTF_Init();

    int timer = 0;
	while (timer != 3000)
	{
		DrawText(&app);
		++timer;
	}

    return 0;
}