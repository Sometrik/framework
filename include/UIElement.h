#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <Element.h>
#include <TextureRef.h>
#include <Context.h>
#include <VBO.h>
#include <TouchEvent.h>
#include <CommandEvent.h>
#include <SysEvent.h>
#include <FWPlatform.h>
 
class UIElement : public Element {
 public:
  UIElement() { }

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
      ev.setHandled();
      ev.requestRedraw();
    } else if (ev.getType() == TouchEvent::ACTION_UP && isTouched()) {
      setTouched(false);
      ev.setHandled();
      ev.requestRedraw();
    }
  }

  void onSysEvent(SysEvent & ev) {
    if (ev.getType() == SysEvent::STOP) {
      clearTexture();
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
  void clearTexture() { texture.reset(); }
  virtual canvas::TextureRef drawContent() = 0;

  float x = 0, y = 0, width = 0, height = 0;
  bool is_touched = false;
  
 private:
  canvas::TextureRef texture;
};

#endif
