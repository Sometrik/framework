#ifndef _CANVASELEMENT_H_
#define _CANVASELEMENT_H_

#include <Element.h>
#include <TextureRef.h>
 
class CanvasElement : public Element {
 public:
  CanvasElement() { }

  void setX(float _x) { x = _x; }
  void setY(float _y) { y = _y; }
  void setWidth(float _width) { width = _width; }
  void setHeight(float _height) { height = _height; }

  void onDrawEvent(DrawEvent & ev) {
    if (!texture.get()) {

    }
  }

  virtual void draw() = 0;
  
 private:
  float x = 0, y = 0, width = 0, height = 0;
  canvas::TextureRef texture;
};

#endif
