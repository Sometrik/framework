#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include <CanvasElement.h>
#include <PrimitiveRenderer.h>

class CButton : public CanvasElement {
 public:
  CButton(const std::string & _label) : label(_label) { }

  canvas::TextureRef drawContent() override {
    auto context = getPlatform().createContextFactory()->createContext(width, height, canvas::InternalFormat::RGBA8, true);
    if (is_touched) {
      context->fillStyle = "#e80";
      context->fillRect(0, 0, width, height);
    } else {
      context->strokeStyle = "#e80";
      context->strokeRect(0, 0, width, height);
    }
    context->fillStyle = "#fff";
    context->font.size = 20;
    context->fillText(label, width / 2, height / 2);    
    return canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  }

 private:
  std::string label;
  // float highlight = 0.0f;
};

#endif
