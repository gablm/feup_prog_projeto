#include "Image.hpp"

namespace prog
{
  //Default constructor
  Image::Image(int w, int h, const Color &fill)
  { 
    // Define the width and heigh of the image
    this->w = w;
    this->h = h;
    int filling_pixels = w * h;

    // Create a memory allocated vector
    std::vector<Color> *temp = new std::vector<Color>;

    // Fill all spots with the specified color
    for (int i = 1; i <= filling_pixels; i++) {
        temp->push_back(fill);
    }

    // Asign the new MA vector to pixels
    pixels = temp;
  }

  //Destructor
  Image::~Image(){ delete pixels; }

  // Accessors
  int Image::width() const { return w; }
  int Image::height() const { return h; }

  // Get a mutable reference of RGB values of a pixel specified by coordinates (x,y)
  Color& Image::at(int x, int y){ 
    return pixels->at(y * w + x); }

  // Get the RGB values of a pixel specified by coordinates (x,y)
  const Color& Image::at(int x, int y) const { 
    return pixels->at(y * w + x);
  }
}
