#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <FWRect.h>
#include <FWActionSheet.h>
#include <FWApplication.h>
#include <EventBase.h>
#include <Message.h>
#include <Logger.h>
#include <SoundCanvas.h>
#include <PrimitiveRenderer.h>
#include <FWDefs.h>

#include <string>
#include <memory>

class FWApplication;
class EventLoop;

namespace canvas {
  class ContextFactory;
};
class HTTPClientFactory;

class FWPlatform {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };
  
 FWPlatform(float _display_scale, const char * _glsl_version, bool _has_es3)
   : display_scale(_display_scale), glsl_version(_glsl_version), has_es3(_has_es3) { }
  virtual ~FWPlatform() { }

  const std::string & getGLSLVersion() const { return glsl_version; }
  
  virtual std::shared_ptr<EventLoop> createEventLoop() { return std::shared_ptr<EventLoop>(0); }
  virtual void showMessageBox(const std::string & title, const std::string & message) = 0;
  virtual std::string showTextEntryDialog(const std::string & message) = 0;
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual double getTime() const = 0;
  virtual std::shared_ptr<canvas::ContextFactory> createContextFactory() const = 0;
  virtual std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
  virtual void storeValue(const std::string & key, const std::string & value) = 0;
  virtual std::string loadValue(const std::string & key) = 0;
  virtual int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) = 0;
  
  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }
  
  void launchBrowser(const std::string & input_url) {
    sendMessage(Message(Message::LAUNCH_BROWSER, input_url));
  }

  void postNotification(const std::string & title, const std::string & message) {
    sendMessage(Message(Message::POST_NOTIFICATION, title, message));
}

  void setApplication(FWApplication * _application) {application = _application;}
  FWApplication & getApplication() { return *application; }
  SoundCanvas & getSoundCanvas() {
    if (soundCanvas == 0){
      soundCanvas = createSoundCanvas();
    }
    return *soundCanvas;
  }
  Logger & getLogger() {
    if (logger == 0) {
      logger = createLogger();
    }
    return *logger;
  }
  virtual void sendMessage(const Message & message) {
    if (message.getType() == Message::SHOW_VIEW ||
	(!activeViewId && (message.getType() == Message::CREATE_FORMVIEW || message.getType() == Message::CREATE_OPENGL_VIEW))) {
      activeViewId = message.getValue();
    }
  }

  const std::shared_ptr<PrimitiveRenderer> & getRenderer() { return renderer; }
  void setRenderer(const std::shared_ptr<PrimitiveRenderer> & _renderer) { renderer = _renderer; }
  
  void postEvent(EventBase & ev) {
    auto e = getApplication().getFirstChild();
    ev.dispatch(*e);
  }
  
  int getNextElementId(){
    return nextElementId++;
  }
  void setDisplayWidth(int w) { display_width = w; }
  void setDisplayHeight(int h) { display_height = h; }

  int getDisplayWidth() const { return display_width; }
  int getDisplayHeight() const { return display_height; }
  float getDisplayScale() const { return display_scale; }
  bool hasES3() const { return has_es3; }
  int getActiveViewId() const { return activeViewId; }
  
 protected:
  virtual std::shared_ptr<SoundCanvas> createSoundCanvas() const = 0;
  virtual std::shared_ptr<Logger> createLogger() const = 0;
  int display_width = 0, display_height = 0;
  float display_scale = 1.0f;
  std::string glsl_version;
  bool has_es3 = false;
  FWApplication * application = 0;
    
 private:
  std::shared_ptr<SoundCanvas> soundCanvas;
  std::shared_ptr<Logger> logger;
  std::shared_ptr<PrimitiveRenderer> renderer;
  int nextElementId = 1;
  int activeViewId = 0;
};

#endif
