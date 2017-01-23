#ifndef _OPENGLGRAPHICSCONTEXT_H_
#define _OPENGLGRAPHICSCONTEXT_H_

#include <GraphicsContext.h>

#include <PrimitiveRenderer.h>
#include <OpenGLTexture.h>
#include <VBO.h>

namespace canvas {
  class Texture;
};

class OpenGLGraphicsContext : public GraphicsContext {
 public:

  bool hasContent() const override {
    return texture.get() != nullptr;
  }

  void draw(float x, float y, float width, float height) {
    VBO vbo;
    vbo.quad2d(x, y,
	       x, y + height,
	       x + width, y + height,
	       x + width, y
	       );
    glm::mat4 mat(1.0f);
    renderer->renderTexturedWindow(vbo, *texture, mat);
  }

  void renderButton(float width, float height, bool has_highlight) override {
    auto context = getPlatform().createContextFactory()->createContext(width, height, canvas::InternalFormat::RGBA8);

    float x = 0.0f, y = 0.0f, w = width - 1, h = height - 1, r = 5.0f;
    context->moveTo(x+r, y);
    context->arcTo(x+w, y,   x+w, y+h, r);
    context->arcTo(x+w, y+h, x,   y+h, r);
    context->arcTo(x,   y+h, x,   y,   r);
    context->arcTo(x,   y,   x+w, y,   r);
    if (has_highlight) {
      context->fillStyle = "#730";
      context->fill();
    }
    context->strokeStyle = "#e80";
    context->stroke();
    context->fillStyle = "#fff";
    context->font.size = 20;
    context->textAlign = "center";
    context->textBaseline = "middle";
    context->fillText(label, width / 2, height / 2);    
    return canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  }
  
  void clear() override {
    texture.reset();
  }
  
 private:
  std::shared_ptr<canvas::Texture> texture;
  std::shared_ptr<PrimitiveRenderer> renderer;
};

#endif
