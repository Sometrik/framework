#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include <CanvasElement.h>
#include <PrimitiveRenderer.h>

class CButton : public CanvasElement {
 public:
  CButton(const std::string & _label) : label(_label) { }

#if 0
  canvas::TextureRef drawContent() {
    auto context = getContextFactory().createContext(width, height, canvas::InternalFormat::RGBA8, true);
    context->font.size = 20;
    context->fillText(label, width / 2, height / 2);
    return canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  }
#endif

 private:
  std::string label;
  float highlight = 0.0f;
};

#endif
