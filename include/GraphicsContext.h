#ifndef _GRAPHICSCONTEXT_H_
#define _GRAPHICSCONTEXT_H_

class GraphicsContext {
 public:
  virtual ~GraphicsContext() = default;
  
  virtual bool hasContent() const = 0;
  virtual void draw(float x, float y, float width, float height) = 0;
  virtual void clear() = 0;
  virtual void renderButton(float width, float height, bool has_highlight) = 0;
};

class NullGraphicsContext : public GraphicsContext {
 public:
  bool hasContent() const override { return false; }
  void draw(float x, float y, float width, float height) override { }  
  void clear() override { }
  void renderButton(float width, float height, bool has_highlight) { }
};

#endif
