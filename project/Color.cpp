#include "Color.hpp"

namespace prog {
    Color::Color() {
        r = 0; b = 0; g = 0;
    }
    Color::Color(const Color& other) {
        r = other.r; b = other.b; g= other.g;
    }
    Color::Color(rgb_value red, rgb_value green, rgb_value blue) {
        r = red; g = green; b = blue;
    }
    rgb_value Color::red() const {
        return r;
    }
    rgb_value Color::green() const {
        return g;
    }
    rgb_value Color::blue() const {
        return b;
    } 

    rgb_value& Color::red()  {
        return r;
    }
    rgb_value& Color::green()  {
        return g;
    }
    rgb_value& Color::blue()  {
        return b;
    }
}
