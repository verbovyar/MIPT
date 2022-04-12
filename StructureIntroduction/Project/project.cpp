#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

#include <SDL2/SDL.h>


double RandomDouble(double min_num, double max_num) {
    double rand_num = static_cast<double>(rand()) / RAND_MAX;
    return min_num + rand_num * (max_num - min_num);
}


struct Coord {
    Coord(double x, double y)
        : x(x), y(y) {
    }

    Coord operator+(const Coord& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Coord operator-(const Coord& rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Coord operator*(double mult) const {
        return {x * mult, y * mult};
    }

    Coord operator/(double mult) const {
        if (mult == 0) {
            std::abort();
        }
        return {x / mult, y / mult};
    }

    double x = 0;
    double y = 0;
};


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


class IFigure {
public:
    virtual std::string Description() const = 0;

    virtual Coord MassCenter() const = 0;

    virtual void Draw(std::shared_ptr<GraphicsManager> manager) const = 0;
};


class Triangle : public IFigure {
public:
    Triangle(const Coord& a, const Coord& b, const Coord& c)
        : a_(a), b_(b), c_(c) {
    }

    std::string Description() const override {
        return "Triangle";
    }

    Coord MassCenter() const override {
        return (a_ + b_ + c_) / 3;
    }

    void Draw(std::shared_ptr<GraphicsManager> manager) const override {
        manager->DrawLine(a_, b_);
        manager->DrawLine(b_, c_);
        manager->DrawLine(c_, a_);
        manager->Present();
    }

private:
    Coord a_;
    Coord b_;
    Coord c_;
};


class Rectangle : public IFigure {
public:
    Rectangle(Coord a, Coord c)
        : a_(a)
        , b_((a.x + c.x) / 2 + (a.y - c.y) / 2, (a.y + c.y) / 2 + (a.x - c.x) / 2)
        , c_(c)
        , d_((a.x + c.x) / 2 - (a.y - c.y) / 2, (a.y + c.y) / 2 - (a.x - c.x) / 2) {
    }

    std::string Description() const override {
        return "Rectangle";
    }

    Coord MassCenter() const override {
        return (a_ + c_) / 2;
    }

    void Draw(std::shared_ptr<GraphicsManager> manager) const override {
        manager->DrawLine(a_, b_);
        manager->DrawLine(b_, c_);
        manager->DrawLine(c_, d_);
        manager->DrawLine(d_, a_);
        manager->Present();
    }

private:
    Coord a_;
    Coord b_;
    Coord c_;
    Coord d_;
};


class FigureFactory {
public:
    FigureFactory(double x_lim, double y_lim)
        : x_lim_(x_lim), y_lim_(y_lim) {
    }

    std::shared_ptr<Triangle> CreateRandomTriangle() {
        Coord center(std::rand() % x_lim_, std::rand() % y_lim_);
        
        Coord a = Coord(RandomDouble(-triangle_max_, triangle_max_), RandomDouble(-triangle_max_, triangle_max_));
        Coord b = Coord(RandomDouble(-triangle_max_, triangle_max_), RandomDouble(-triangle_max_, triangle_max_));
        Coord c = Coord(RandomDouble(-triangle_max_, triangle_max_), RandomDouble(-triangle_max_, triangle_max_));

        return std::make_shared<Triangle>(center + a, center + b, center + c);
    }

    std::shared_ptr<Rectangle> CreateRandomRectangle() {
        Coord center(RandomDouble(0, x_lim_), RandomDouble(0, y_lim_));

        Coord a = Coord(RandomDouble(-rectangle_radius_max_, rectangle_radius_max_), RandomDouble(-rectangle_radius_max_, rectangle_radius_max_));
        Coord c = Coord(RandomDouble(-rectangle_radius_max_, rectangle_radius_max_), RandomDouble(-rectangle_radius_max_, rectangle_radius_max_));

        return std::make_shared<Rectangle>(center + a, center + c);
    }

    std::shared_ptr<IFigure> CreateRandomFigure() {
        int figure_num = std::rand() % 2;
        switch (figure_num) {
        case 0: // Triangle
            return CreateRandomTriangle();
        case 1: // Rectangle
            return CreateRandomRectangle();
        default:
            std::abort();
        }
    }

private:
    int x_lim_;
    int y_lim_;

    const double triangle_max_ = 50;
    const double rectangle_radius_max_ = 50;
};


int main() {
    double width = 1000;
    double height = 500;
    int sleep_delay = 2;

    std::shared_ptr<GraphicsManager> graphics_manager = std::make_shared<GraphicsManager>(width, height);

    std::shared_ptr<FigureFactory> figure_factory = std::make_shared<FigureFactory>(width, height);

    std::vector<std::shared_ptr<IFigure>> figures;
    
    for (int i = 0; i < 30; i++) {
        auto figure = figure_factory->CreateRandomFigure();
        figures.emplace_back(figure);
        figure->Draw(graphics_manager);

        sleep(sleep_delay);
    }

    for (auto& figure: figures) {
        std::cout << figure->Description() << std::endl;
    }

    return 0;
}