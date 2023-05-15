#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        return nullptr;
    }

    void saveToXPM2(const std::string& file, const Image* image) {

    }

    bool operator==(const Color& a, const Color& b){
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}
