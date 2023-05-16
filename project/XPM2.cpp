#include "XPM2.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

namespace prog {

    // Loads an image in XPM2 format
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
        for (int i = 0; i < colors; i++){
            getline(in, line);
            istringstream word(line);
            char ch, skip;
            string color_hex;
            word >> ch >> skip >> color_hex;
            colormap.insert({ch, hex_to_number(color_hex)});
        }

        for (int j = 0; j < height; j++){
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

    // Returns the color of an hex value in string form
    Color hex_to_number(string color_hex){
        int red = stoi(color_hex.substr(1, 2), nullptr, 16);
        int green = stoi(color_hex.substr(3, 2), nullptr, 16);
        int blue = stoi(color_hex.substr(5, 2), nullptr, 16);
        return Color(red, green, blue);
    }

    // Saves an image to XPM2 format
    void saveToXPM2(const string& file, const Image* image) {
        std::ofstream fout(file);
        std::vector<Color> cores; char c;
        std::vector<std::string> linhas;

        for (int j = 0; j < image->height(); j++){

            std::ostringstream out;

            for (int i = 0; i < image->width(); i++){

                Color cor = image->at(i, j);
                auto itr = std::find(cores.begin(), cores.end(), cor);

                if (itr == cores.end()){
                    c = 48 + cores.size();
                    cores.push_back(cor);
                }else{
                    c = 48 + itr - cores.begin(); 
                }
                out << c;
            }

            linhas.push_back(out.str());
        }

        fout << "! XPM2\n" << image->width() << " " << image->height() << " " << cores.size() << " 1\n";
        char car = 48;
        for (Color c: cores){
            std::stringstream out;
            out << std::setfill('0') << std::setw(2) << std::hex << (0xff & c.red()) << std::setfill('0') << std::setw(2) << (0xff & c.green()) << std::setfill('0') << std::setw(2) << (0xff & c.blue());
            fout << car << " c #" << out.str() << "\n";
            car++;
        }
        
        for (std::string c: linhas){
            fout << c << "\n";
        }
    }

    bool operator==(const Color& a, const Color& b){
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}
