#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            }

            if (command == "invert") {
                invert();
                continue;
            }
            
            if (command == "to_gray_scale") {
                to_gray_scale();
                continue;
            }
            if (command == "replace") {
                replace(r1, g1, b1, r2, g2, b2);
                continue;
            }
            // TODO ...

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    
    void Script::invert(){
        // Loops thru each x and y
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                /* Reverts each RGB value (at, blue, green and red are all overloaded 
                with both an mutable reference function and a function that only returns its values.) */
                image->at(i, j).blue() = 255 - image->at(i, j).blue();
                image->at(i, j).green() = 255 - image->at(i, j).green();
                image->at(i, j).red() = 255 - image->at(i, j).red();
            }
        }
    }
    void Script::to_gray_scale(){
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                int v = (image->at(i,j).red() + image->at(i, j).green() + image->at(i, j).blue())/3;
                image->at(i, j).blue() = v;
                image->at(i, j).green() = v;
                image->at(i, j).red() = v;
            }        
        }
    }
    void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2){
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                if(image->at(i, j).blue() != b1 || image->at(i, j).green() != g1 || image->at(i, j).red() != r1) continue;
                image->at(i, j).blue() = b2;
                image->at(i, j).green() = g2;
                image->at(i, j).red() = r2;
            }        
        }        
    }
}
