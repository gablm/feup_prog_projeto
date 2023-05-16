#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

namespace prog {
    Image* loadFromXPM2(const string& file) {
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



    void saveToXPM2(const string& file, const Image* image) {
        ofstream out(file);
        out << "! XPM2\n";
        map <Color, char> colormap;
        char c = '0';
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++) {
                Color temp = image->at(i, j);
                map<Color, char>::iterator itr = colormap.find(temp);
                if (itr != colormap.end()) {
                    colormap.insert({temp, c});
                    c++;
                }
            }
        }
        out << image->width() << " " << image->height() << " " << colormap.size() << " 1";
        for (auto color : colormap) {
            out << color.second << " c #" << number_to_hex(color.first);
        }
        for (int j = 0; j < image->height(); j++){
            out << '\n';
            for (int i = 0; i < image->width(); i++) {
                map<Color, char>::iterator itr = colormap.find(image->at(i, j));
                out << itr->second;
            }
        }
    }

    string number_to_hex(Color color) {
        ostringstream result;
        result << setfill('0') << setw(2) << hex << color.red() 
                << setfill('0') << setw(2) << hex << color.green() 
                << setfill('0') << setw(2) << hex << color.blue();
        return result.str();
    }

    bool operator==(const Color& a, const Color& b){
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}
