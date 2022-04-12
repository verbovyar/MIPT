#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

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