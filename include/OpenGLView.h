#ifndef _OPENGLVIEW_H_
#define _OPENGLVIEW_H_

#include <FWViewBase.h>
#include <Message.h>
#include <FWPlatformBase.h>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

class OpenGLView : public FWViewBase {
 public:
  OpenGLView() { }

  void initialize(FWPlatformBase * _platform) override;

  bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) {
    logical_width = _logical_width;
    logical_height = _logical_height;
    actual_width = _actual_width;
    actual_height = _actual_height;
    return false;
  }

  int getLogicalWidth() const { return logical_width; }
  int getLogicalHeight() const { return logical_height; }
  int getActualWidth() const { return actual_width; }
  int getActualHeight() const { return actual_height; }

 private:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
};

#endif
