#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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
                continue;
            }
            
            // XPM2 file loading and saving command handling
            if (command == "xpm2_open"){
                clear_image_if_any();
                string file;
                input >> file;
                image = loadFromXPM2(file);
                continue;
            }

            if (command == "xpm2_save"){
                string file;
                input >> file;
                saveToXPM2(file, image);
                continue;
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

    /* Reverts each RGB value (at, blue, green and red are all overloaded 
        with both an mutable reference function and a function that only returns its values.) */
    void Script::invert(){
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                image->at(i, j).blue() = 255 - image->at(i, j).blue();
                image->at(i, j).green() = 255 - image->at(i, j).green();
                image->at(i, j).red() = 255 - image->at(i, j).red();
            }
        }
    }

    // Converts a image to its gray-scaled version
    void Script::to_gray_scale(){
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                int v = (image->at(i,j).red() + image->at(i, j).green() + image->at(i, j).blue())/3;
                image->at(i, j) = Color(v, v, v);
            }        
        }
    }

    // Replaces all pixels with the RGB values (r1, g1, b1) with the values (r2, g2. b2)
    void Script::replace(int r1, int g1, int b1, int r2, int g2, int b2){
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                if(image->at(i, j).blue() != b1 || image->at(i, j).green() != g1 || image->at(i, j).red() != r1) continue;
                image->at(i, j) = Color(r2, g2, b2);
            }        
        }        
    }

    /* Fills all pixels in a rectagle define by its top-left corner (x,y), 
        width (w) and height (h) with the color with RGB values (r,g,b) */
    void Script::fill(int x, int y, int w, int h, int r, int g, int b){
        for (int i = x; i < x+w; i++){
            for (int j = y; j < y+h; j++){
                image->at(i, j) = Color(r,g,b);
            }
        }
    }

    // Mirrors the image horizontally
    void Script::h_mirror(){
        Color tempcolor;     
        for (int i = 0; i < image->width()/2; i++){
            for (int j = 0; j < image->height(); j++){
                tempcolor = image->at(i,j);
                image->at(i,j) = image->at(image->width()-i-1,j);
                image->at(image->width()-i-1,j) = tempcolor;
            }        
        }          
    }

    // Mirrors the image vertically
    void Script::v_mirror(){    
        Color tempcolor;     
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height()/2; j++){
                tempcolor = image->at(i,j);
                image->at(i,j) = image->at(i,image->height()-1-j);
                image->at(i,image->height()-1-j) = tempcolor;
            }        
        }          
    }
    
    /* Copies all pixels from an image to the current image, starting in the
        top-left corner (x,y) ignoring all pixels with the RGB value specified*/
    void Script::add(string filename, int r, int g, int b, int x, int y){
        Image *tempimage = loadFromPNG(filename);
        for (int i = x; i < x + tempimage->width(); i++){
            for (int j = y; j < y + tempimage->height(); j++){
                if(tempimage->at(i-x, j-y).blue() == b && tempimage->at(i-x, j-y).green() == g && tempimage->at(i-x, j-y).red() == r) continue;
                image->at(i, j) = tempimage->at(i-x, j-y);             
            }
        }
        delete tempimage;
    }

    /* Crops an image, creating a new one based on the top-left corner position 
        on the image and the new image's width and height */
    void Script::crop(int x, int y, int w, int h){
        Image *tempimage = new Image(w, h);
        for (int i = 0; i < w; i++){
            for (int j = 0; j < h; j++){
                tempimage->at(i, j) = image->at(i+x, j+y);
            }
        }
        clear_image_if_any();
        image = tempimage;
    }

    // Rotates the image left by 90 degrees
    void Script::rotate_left(){
        Image *tempimage = new Image(image->height(), image->width());
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                tempimage->at(j, tempimage->height() - i - 1) = image->at(i, j);
            }
        }
        delete image;
        image = tempimage;
    }

    // Rotates the image right by 90 degrees
    void Script::rotate_right(){ 
        Image *tempimage = new Image(image->height(), image->width());
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                tempimage->at(tempimage->width() - j - 1, i) = image->at(i, j);
            }
        }
        clear_image_if_any();
        image = tempimage;
    }

    // Replaces each pixel in a image with the median color within a "ws"-side square where that pixel is its center 
    void Script::median_filter(int ws){
        Image *tempimage = new Image(image->width(), image->height());
        for (int i = 0; i < image->width(); i++){
            for (int j = 0; j < image->height(); j++){
                tempimage->at(i, j) = median_color(ws, i, j);
            }
        }
        clear_image_if_any();
        image = tempimage;
    }

    // Finds the median color in a range ws with the center in the coordinate (x,y)
    Color Script::median_color(int ws, int x, int y){
        int range = (ws-1)/2; Color tempcolor;
        std::vector<int> m_r; std::vector<int> m_g; std::vector<int> m_b;

        for(int i = x - range; i <= x + range; i++){
            for (int j = y - range; j <= y + range; j++){
                if(i >= 0 && i < image->width() && j >= 0 && j < image->height()){
                    tempcolor = image->at(i, j);
                    m_r.push_back(tempcolor.red());
                    m_g.push_back(tempcolor.green());
                    m_b.push_back(tempcolor.blue());
                }
            } 
        }

        sort(m_r.begin(), m_r.end());
        sort(m_g.begin(), m_g.end());
        sort(m_b.begin(), m_b.end());
        
        int middle = m_r.size() / 2;
        int red, green, blue;

        if (m_r.size() % 2 != 0){
            red = m_r[middle];
            green = m_g[middle];
            blue = m_b[middle];
        }else{
            red = (m_r[middle-1] + m_r[middle]) / 2;
            green = (m_g[middle-1] + m_g[middle]) / 2;
            blue = (m_b[middle-1] + m_b[middle]) / 2; 
        }

        return Color(red, green, blue);
    }
}
