#include "Color.hpp"

namespace prog{
    // Default constructor
    Color::Color(){r = 0; b = 0; g = 0;}

    // Copy constructor
    Color::Color(const Color& other){ 
        r = other.r; b = other.b; g= other.g;}

    // Constructor with the RGB colors as arguments    
    Color::Color(rgb_value red, rgb_value green, rgb_value blue){ 
        r = red; g = green; b = blue;
    }

    // Accessors
    rgb_value Color::red() const{ return r; }
    rgb_value Color::green() const{ return g; }
    rgb_value Color::blue() const{ return b; } 

    // Mutable references to each color
    rgb_value& Color::red(){ return r; }
    rgb_value& Color::green(){ return g; }
    rgb_value& Color::blue(){ return b; }
}
