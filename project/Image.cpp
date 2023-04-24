#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    this->w = w;
    this->h = h;
    int filling_pixels = w * h;
    std::vector<Color> *temp = new std::vector<Color>;
    for (int i = 1; i <= filling_pixels; i++) {
        temp->push_back(fill);
    }
    pixels = temp;
  }
  Image::~Image()
  {
    delete pixels;
  }
  int Image::width() const
  {
    return w;
  }
  int Image::height() const
  {
    return h;
  }

  
  Color& Image::at(int x, int y)
  { 
    return pixels->at(y * w + x);
  }

  const Color& Image::at(int x, int y) const
  {
    return pixels->at(y * w + x);
  }
}
