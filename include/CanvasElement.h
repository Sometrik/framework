#ifndef _CANVASELEMENT_H_
#define _CANVASELEMENT_H_

#include <Element.h>
#include <TextureRef.h>
#include <Context.h>
#include <VBO.h>
#include <TouchEvent.h>
#include <CommandEvent.h>
 
class CanvasElement : public Element {
 public:
  CanvasElement() { }

  void setX(float _x) { x = _x; }
  void setY(float _y) { y = _y; }
  void setWidth(float _width) { width = _width; }
  void setHeight(float _height) { height = _height; }

  void onDrawEvent(DrawEvent & ev) override {
    if (!texture.get()) {
      texture = drawContent();
    }
    VBO vbo;
    vbo.quad2d(x, y,
	       x, y + height,
	       x + width, y + height,
	       x + width, y
	       );
    glm::mat4 mat(1.0f);
    auto & renderer = getPlatform().getRenderer();
    renderer->renderTexturedWindow(vbo, texture, mat);
  }

  void onTouchEvent(TouchEvent & ev) override {
    if (ev.getType() == TouchEvent::ACTION_DOWN &&
	ev.getX() >= x && ev.getX() < x + width &&
	ev.getY() >= y && ev.getY() < y + height) {
      setTouched(true);
      CommandEvent ev2(ev.getTimestamp(), getId());
      ev2.dispatch(*this);
    } else if (ev.getType() == TouchEvent::ACTION_UP) {
      setTouched(false); 
    }
  }

  void setTouched(bool t) {
    if (t != is_touched) {
      is_touched = t;
      clearTexture();
    }
  }

  bool isTouched() const { return is_touched; }

 protected:
  void clearTexture() { texture.clear(); }
  virtual canvas::TextureRef drawContent() = 0;

  float x = 0, y = 0, width = 0, height = 0;
  bool is_touched = false;
  
 private:
  canvas::TextureRef texture;
};

#endif
