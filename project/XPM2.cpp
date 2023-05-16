#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

namespace prog {
    Image* loadFromXPM2(const std::string& file) {
        ifstream in(file);
        string line;
        getline(in, line);
        getline(in, line);
        istringstream word(line);
        int width, height, colors;
        word >> width >> height >> colors;
        Image *image = new Image(width, height);
        map <char, Color> colormap;
        for(int i = 0; i<colors; i++){
            getline(in, line);
            istringstream word(line);
            char c, trash;
            string colorhex;
            word >> c >> trash >> colorhex;
            
        }
    }

    Color hex_to_number(string colorhex){
        
    }

    void saveToXPM2(const std::string& file, const Image* image) {

    }

    bool operator==(const Color& a, const Color& b){
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}
