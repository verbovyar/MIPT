#include "figures.h"

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