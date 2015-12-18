#ifndef _FWRECT_H_
#define _FWRECT_H_

class FWRect {
 public:
  FWRect() : x(0), y(0), width(0), height(0) { }
  FWRect(float _x, float _y, float _width, float _height) : x(_x), y(_y), width(_width), height(_height) { }
  float x, y, width, height;
};

#endif
