#ifndef __prog_XPM2_hpp__
#define __prog_XPM2_hpp__

#include <string>
#include "Image.hpp"

namespace prog {
    // XPM2 file loading and saving
    Image* loadFromXPM2(const std::string &file);
    void saveToXPM2(const std::string& file, const Image* img);

    // Auxiliar functions
    Color hexToColor(std::string colorhex);
}
#endif
