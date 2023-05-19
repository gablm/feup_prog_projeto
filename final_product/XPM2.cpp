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
        getline(in, line); // Ignores header, always the same
        getline(in, line);
        
        istringstream word(line);
        int width, height, colors;
        word >> width >> height >> colors;
        Image *image = new Image(width, height);

        map <char, Color> colormap;
        for (int i = 0; i < colors; i++) {
            getline(in, line);
            istringstream word(line);
            char c, skip;
            string color_hex;
            word >> c >> skip >> color_hex;
            colormap.insert({c, hexToColor(color_hex)});
        }

        for (int j = 0; j < height; j++) {
            getline(in, line);
            istringstream word(line);
            char a;
            for(int i = 0; i < width; i++) {
                word >> a;
                map <char, Color>::iterator pixel = colormap.find(a);
                image->at(i,j) = pixel->second;
            }
        }

        return image;
    }

    // Returns the color of an hex value in string form
    Color hexToColor(string color_hex) {
        int red = stoi(color_hex.substr(1, 2), nullptr, 16);
        int green = stoi(color_hex.substr(3, 2), nullptr, 16);
        int blue = stoi(color_hex.substr(5, 2), nullptr, 16);
        return Color(red, green, blue);
    }

    // Saves an image to XPM2 format
    void saveToXPM2(const string& file, const Image* image) {
        ofstream fout(file);
        vector<Color> colors; char c;
        vector<string> lines;

        for (int j = 0; j < image->height(); j++) {
            ostringstream out;
            for (int i = 0; i < image->width(); i++) {
                Color color = image->at(i, j);
                auto colorIterator = find(colors.begin(), colors.end(), color);
                if (colorIterator == colors.end()) {
                    c = 48 + colors.size();
                    colors.push_back(color);
                }
                else {
                    c = 48 + colorIterator - colors.begin();
                }
                out << c;
            }
            lines.push_back(out.str());
        }

        // Prints the file, following the steps: Headers -> Hex-char associations -> Image Pixels
        fout << "! XPM2\n" << image->width() << " " << image->height() << " " << colors.size() << " 1\n";
        char ColorChar = 48;
        for (Color c: colors) {
            stringstream out;
            out << setfill('0') << setw(2) << hex << (0xff & c.red()) << setfill('0') << setw(2) << (0xff & c.green()) << setfill('0') << setw(2) << (0xff & c.blue());
            fout << ColorChar << " c #" << out.str() << "\n";
            ColorChar++;
        }

        for (string c: lines) {
            fout << c << "\n";
        }
    }

    bool operator==(const Color& a, const Color& b) {
        return a.blue() == b.blue() && a.green() == b.green() && a.red() == b.red();
    }
}