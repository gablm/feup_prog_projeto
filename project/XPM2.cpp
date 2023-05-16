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
            colormap.insert({c, hex_to_number(colorhex)});
        }
        for(int j = 0; j < height; j++){
            getline(in, line);
            istringstream word(line);
            char a;
            for(int i = 0; i < width; i++){
                word >> a;
                map <char, Color>::iterator pixel = colormap.find(a);
                image->at(i,j) = pixel->second;
            }
        }
        return image;
    }

    Color hex_to_number(string colorhex){
        int red, green, blue;
        red = stoi(colorhex.substr(1, 2), nullptr, 16);
        green = stoi(colorhex.substr(3, 2), nullptr, 16);
        blue = stoi(colorhex.substr(5, 2), nullptr, 16);
        return Color(red, green, blue);
    }



    void saveToXPM2(const std::string& file, const Image* image) {
        
    }





    bool operator==(const Color& a, const Color& b){
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}
