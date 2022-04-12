#include "coord.h"
#include <SDL2/SDL.h>

class GraphicsManager {
public:
    GraphicsManager(int width, int height)
        : window_width(width), window_height(height) {

        int renderer_flags = SDL_RENDERER_ACCELERATED; 
        int window_flags   = 0;

        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cout << "Couldn't initialize SDL: " << SDL_GetError() << "\n";
            std::abort();
        }

        window = SDL_CreateWindow("Graphics", SDL_WINDOWPOS_UNDEFINED, 
                                              SDL_WINDOWPOS_UNDEFINED, 
                                              window_width,
                                              window_height, 
                                              window_flags);

        if (!window) {
            std::cout << "Failed to open window: " << SDL_GetError() << "\n";
            std::abort();
        }

        // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

        renderer = SDL_CreateRenderer(window, -1, renderer_flags);
        
        if (!renderer) {
            printf("Failed to create renderer: %s\n", SDL_GetError());
            std::abort();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void DrawLine(const Coord& lhs, const Coord& rhs) {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

        int result = SDL_RenderDrawLine(renderer, lhs.x, lhs.y, rhs.x, rhs.y);
    }

    void Present() {
        SDL_RenderPresent(renderer);
    }

private:
    int window_width;
    int window_height;

    SDL_Renderer *renderer = nullptr;
    SDL_Window   *window   = nullptr;
    SDL_Surface  *surface  = nullptr;
    SDL_Texture  *texture  = nullptr;
};