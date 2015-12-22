#ifndef _FWCONTEXTBASE_H_
#define _FWCONTEXTBASE_H_

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

class FWPlatformBase;

class FWContextBase {
public:
 FWContextBase(FWPlatformBase * _platform) : platform(_platform) { }
  virtual ~FWContextBase() { }
    
  bool createWindow(const char * title, int requested_width, int requested_height);
  
  virtual bool Init() = 0;
  virtual void onDraw() = 0;
  virtual void onShutdown() = 0;
  virtual void onMemoryWarning() { }
  virtual void onCmdLine(int argc, char *argv[]) { }

  virtual bool onKeyPress(char c, double timestamp, int x, int y) { return false; }
  virtual bool onUpdate(double timestamp) { return false; }
  virtual bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) {
    logical_width = _logical_width;
    logical_height = _logical_height;
    actual_width = _actual_width;
    actual_height = _actual_height;
    return false;
  }
  
  virtual bool touchesBegin(int x, int y, double timestamp, long long identifier) { return false; }
  virtual bool touchesEnded(int x, int y, double timestamp, long long identifier) { return false; }
  virtual bool touchesMoved(int x, int y, double timestamp, long long identifier) { return false; }
  virtual bool flushTouches(int mode, double timestamp) { return false; }
  virtual bool onShake() { return false; }
  
  virtual void loadEvents() { }
  
  int getLogicalWidth() const { return logical_width; }
  int getLogicalHeight() const { return logical_height; }
  int getActualWidth() const { return actual_width; }
  int getActualHeight() const { return actual_height; }
  
  FWPlatformBase & getPlatform() { return *platform; }
  const FWPlatformBase & getPlatform() const { return *platform; }
  
  void refresh() { refresh_requested = true; }
  void clearRefresh() { refresh_requested = false; }
  bool isRefreshRequested() const { return refresh_requested; }
  
  void setWindowSize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) {
    logical_width = _logical_width;
    logical_height = _logical_height;
    actual_width = _actual_width;
    actual_height = _actual_height;
  }

 protected:
    
 private:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
  FWPlatformBase * platform;
  bool refresh_requested = false;
};

#endif
