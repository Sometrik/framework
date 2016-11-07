#ifndef _CANVASELEMENT_H_
#define _CANVASELEMENT_H_

#include <Element.h>
#include <TextureRef.h>
#include <Context.h>
#include <VBO.h>
 
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
    vbo.quad2d(x, y + height,
	       x, y,
	       x + width, y,
	       x + width, y + height
	       );
    glm::mat4 mat(1.0f);
    auto & renderer = getPlatform().getRenderer();
    renderer->renderTexturedWindow(vbo, texture, mat);
  }

 protected:
  virtual canvas::TextureRef drawContent() = 0;

  float x = 0, y = 0, width = 0, height = 0;
  
 private:
  canvas::TextureRef texture;
  // canvas::ContextFactory factory;
};

#endif
