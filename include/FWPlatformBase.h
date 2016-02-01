#ifndef _FWPLATFORMBASE_H_
#define _FWPLATFORMBASE_H_

#if defined __APPLE__
#include <OpenGLES/ES3/gl.h>
#elif defined _WIN32

#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#include "FWRect.h"
#include "FWActionSheet.h"

#include <string>
#include <memory>

class FWContextBase;

namespace canvas {
  class ContextFactory;
};
class HTTPClientFactory;

#define FBO_COLOR	1
#define FBO_DEPTH	2
#define FBO_STENCIL	4

class FWPlatformBase {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
		CACHE_DATABASE
  };
  
 FWPlatformBase(float _display_scale, const char * _glsl_version, bool _has_es3)
   : display_scale(_display_scale), glsl_version(_glsl_version), has_es3(_has_es3) { }
  virtual ~FWPlatformBase() { }

  const std::string & getGLSLVersion() const { return glsl_version; }
  
  bool createContext(FWContextBase * context, const char * title, int requested_width, int requested_height);

  virtual bool createWindow(FWContextBase * context, const char * title) = 0;
  virtual void showMessageBox(const std::string & title, const std::string & message) = 0;
  virtual std::string showTextEntryDialog(const std::string & message) = 0;
  virtual void postNotification(const std::string & title, const std::string & message) = 0;
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual double getTime() const = 0;
  virtual std::shared_ptr<canvas::ContextFactory> createContextFactory() const = 0;
  virtual std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
  virtual void launchBrowser(const std::string & input_url) = 0;
  virtual void storeValue(const std::string & key, const std::string & value) = 0;
  virtual std::string loadValue(const std::string & key) = 0;
  virtual int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) = 0;
  virtual void createFBO(int flags) { }
  
  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }
  
  float getDisplayScale() const { return display_scale; }
  bool hasES3() const { return has_es3; }
  
 protected:
  int display_width = 0, display_height = 0;
  float display_scale;
  std::string glsl_version;
  bool has_es3 = false;
    
#if !defined __APPLE__ && !defined _WIN32
  // Display handle
  EGLNativeDisplayType eglNativeDisplay;
  
  // Window handle
  EGLNativeWindowType eglNativeWindow;
  
  // EGL display
  EGLDisplay  eglDisplay;
  
  // EGL context
  EGLContext  eglContext;
  
  // EGL surface
  EGLSurface  eglSurface;
#endif
};

#endif
