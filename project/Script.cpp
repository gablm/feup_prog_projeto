#include <iostream>
#include <fstream>
#include <string>
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
            
            if (command == "open"){
                open();
                continue;
            }

            if (command == "blank"){
                blank();
                continue;
            }

            // Other commands require an image to be previously loaded.
            if (command == "save"){
                save();
                continue;
            }

            // Simple image manipulation operations command handling
            if (command == "invert"){
                invert();
                continue;
            }
            
            if (command == "to_gray_scale"){
                to_gray_scale();
                continue;
            }

            if (command == "replace"){
                int r1, g1, b1, r2, g2, b2;
                input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
                replace(r1, g1, b1, r2, g2, b2);
                continue;
            }

            if (command == "fill"){
                int x, y, w, h, r, g, b;
                input >> x >> y >> w >> h >> r >> g >> b;
                fill(x, y, w, h, r, g, b);
                continue;
            }

            if (command == "h_mirror"){
                h_mirror();
                continue;
            }

            if (command == "v_mirror"){
                v_mirror();
                continue;
            }

            // Dimension changing operations command handling
            if (command == "add"){
                int x, y, r, g, b;
                string nome;
                input >> nome >> r >> g >> b >> x >> y;
                add(nome, r, g, b, x, y);
                continue;                              
            }

            if (command == "crop"){
                int x, y, w, h;
                input >> x >> y >> w >> h;
                crop(x, y, w, h);
                continue;
            }

            if (command == "rotate_left"){
                rotate_left();
                continue;
            }

            if (command == "rotate_right"){
                rotate_right();
                continue;
            }

            // Median Filter command handling
            if (command == "median_filter"){
                int ws;
                input >> ws;
                median_filter(ws);
            }
            
            // XPM2 file loading and saving command handling
            if (command == "xpm2_open"){
                string file;
                input >> file;
                image = loadFromXPM2(file);
            }

            if (command == "xpm2_save"){
                string file;
                input >> file;
                saveToXPM2(file, image);
            }
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
    
    // Simple image manipulation

    void Script::invert(){
     /* Reverts each RGB value (at, blue, green and red are all overloaded 
        with both an mutable reference function and a function that only returns its values.) */
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                image->at(i, j).blue() = 255 - image->at(i, j).blue();
                image->at(i, j).green() = 255 - image->at(i, j).green();
                image->at(i, j).red() = 255 - image->at(i, j).red();
            }
        }
    }

    void Script::to_gray_scale(){
        // Converts a image to its gray-scaled version
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
        // Replaces all pixels with the RGB values (r1, g1, b1) with the values (r2, g2. b2)
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                if(image->at(i, j).blue() != b1 || image->at(i, j).green() != g1 || image->at(i, j).red() != r1) continue;
                image->at(i, j).blue() = b2;
                image->at(i, j).green() = g2;
                image->at(i, j).red() = r2;
            }        
        }        
    }

    void Script::fill(int x, int y, int w, int h, int r, int g, int b){
        /* Fills all pixels in a rectagle define by its top-left corner (x,y), 
        width (w) and height (h) with the color with RGB values (r,g,b) */
        for (int j = x; j < x+w; j++){
            for (int i = y; i < y+h; i++){
                image->at(j, i).blue() = b;
                image->at(j, i).green() = g;
                image->at(j, i).red() = r;
            }
        }
    }

    void Script::h_mirror(){
        // Mirrors the image horizontally  
        Color tempcolor;     
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width()/2; i++){
                tempcolor = image->at(i,j);
                image->at(i,j) = image->at(image->width()-i-1,j);
                image->at(image->width()-i-1,j) = tempcolor;
            }        
        }          
    }

    void Script::v_mirror(){
        // Mirrors the image vertically  
        Color tempcolor;     
        for (int j = 0; j < image->height()/2; j++){
            for (int i = 0; i < image->width(); i++){
                tempcolor = image->at(i,j);
                image->at(i,j) = image->at(i,image->height()-1-j);
                image->at(i,image->height()-1-j) = tempcolor;
            }        
        }          
    }
    
    void Script::add(string filename, int r, int g, int b, int x, int y){
        /* Copies all pixels from an image to the current image, starting in the
        top-left corner (x,y) ignoring all pixels with the RGB value specified*/
        Image *tempimage = loadFromPNG(filename);
        for (int j = y; j < y + tempimage->height(); j++){
            for (int i = x; i < x + tempimage->width(); i++){
                if(tempimage->at(i-x, j-y).blue() == b && tempimage->at(i-x, j-y).green() == g && tempimage->at(i-x, j-y).red() == r) continue;
                image->at(i, j) = tempimage->at(i-x, j-y);             
            }
        }
        delete tempimage;
    }

    void Script::crop(int x, int y, int w, int h){
        /* Crops an image, creating a new one based on the top-left corner position 
        on the image and the new image's width and height */
        Image *tempimage = new Image(w, h);
        for (int j = 0; j < h; j++){
            for (int i = 0; i < w; i++){
                tempimage->at(i, j) = image->at(i+x, j+y);
            }
        }
        delete image;
        image = tempimage;
    }

    void Script::rotate_left(){
        // Rotates the image left by 90 degrees
        Image *tempimage = new Image(image->height(), image->width());
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                tempimage->at(j, tempimage->height() - i - 1) = image->at(i, j);
            }
        }
        delete image;
        image = tempimage;
    }

    void Script::rotate_right(){
        // Rotates the image right by 90 degrees
        Image *tempimage = new Image(image->height(), image->width());
        for (int j = 0; j < image->height(); j++){
            for (int i = 0; i < image->width(); i++){
                tempimage->at(tempimage->width() - j - 1, i) = image->at(i, j);
            }
        }
        delete image;
        image = tempimage;
    }

    void Script::median_filter(int ws){

    }
}
