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

  void onDrawEvent(DrawEvent & ev) {
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

  bool onTouchEvent(TouchEvent & ev) {
    if (ev.getType() == TouchEvent::ACTION_DOWN &&
	ev.getX() >= x && ev.getX() < x + width &&
	ev.getY() >= y && ev.getY() < y + height) {
      std::cerr << "hit in " << getId() << "\n";
      CommandEvent ev2(getId());
      ev2.dispatch(*this);
    }
    return false;
  }

 protected:
  virtual canvas::TextureRef drawContent() = 0;

  float x = 0, y = 0, width = 0, height = 0;
  
 private:
  canvas::TextureRef texture;
  // canvas::ContextFactory factory;
};

#endif
