#ifndef _OPENGLVIEW_H_
#define _OPENGLVIEW_H_

#include <FWViewBase.h>
#include <PrimitiveRenderer.h>

#define FBO_COLOR	1
#define FBO_DEPTH	2
#define FBO_STENCIL	4

class OpenGLView : public FWViewBase {
 public:
  OpenGLView(int _id = 0) : FWViewBase(_id) { }

  bool isA(const std::string & className) override {
    if (className == "OpenGLView") return true;
    return FWViewBase::isA(className);
  }

  void onResizeEvent(ResizeEvent & ev) override;
  
  int getLogicalWidth() const { return logical_width; }
  int getLogicalHeight() const { return logical_height; }
  int getActualWidth() const { return actual_width; }
  int getActualHeight() const { return actual_height; }

#if 0
  void createFBO(int flags) { }
#endif

  const std::shared_ptr<PrimitiveRenderer> & getRenderer() { return renderer; }
  void setRenderer(const std::shared_ptr<PrimitiveRenderer> & _renderer) { renderer = _renderer; }
 
 protected:
  void initialize(FWPlatform * _platform) override;
  void checkGLError();

 private:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
  std::shared_ptr<PrimitiveRenderer> renderer;
};

#endif
