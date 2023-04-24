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
    return -1;
  }
  int Image::height() const
  {
    return -1;
  }

  // TODO: remove this DUMMY_color variable once you have appropriate fields for representing image pixels.
  Color DUMMY_color;

  Color& Image::at(int x, int y)
  {
    return DUMMY_color;
  }

  const Color& Image::at(int x, int y) const
  {
    return DUMMY_color;
  }
}
