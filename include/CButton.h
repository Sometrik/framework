#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include <CanvasElement.h>
#include <PrimitiveRenderer.h>

class CButton : public CanvasElement {
 public:
  CButton(const std::string & _label) : label(_label) { }

  canvas::TextureRef drawContent() override {
    auto context = getPlatform().createContextFactory()->createContext(width, height, canvas::InternalFormat::RGBA8, true);

    float x = 0.0f, y = 0.0f, w = width - 1, h = height - 1, r = 5.0f;
    context->moveTo(x+r, y);
    context->arcTo(x+w, y,   x+w, y+h, r);
    context->arcTo(x+w, y+h, x,   y+h, r);
    context->arcTo(x,   y+h, x,   y,   r);
    context->arcTo(x,   y,   x+w, y,   r);
    if (is_touched) {
      context->fillStyle = "#a40";
      context->fill();
    }
    context->strokeStyle = "#e80";
    context->stroke();
    context->fillStyle = "#fff";
    context->font.size = 20;
    context->textAlign = "center";
    context->fillText(label, width / 2, height / 2);    
    return canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  }

 private:
  std::string label;
  // float highlight = 0.0f;
};

#endif
