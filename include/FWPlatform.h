#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <Element.h>
#include <FWApplication.h>
#include <EventBase.h>
#include <Command.h>
#include <Logger.h>
#include <PrimitiveRenderer.h>
#include <FWDefs.h>
#include <FWPreferences.h>

#ifdef HAS_SOUNDCANVAS
#include <SoundCanvas.h>
#endif

#include <sstream>
#include <string>
#include <memory>

namespace canvas {
  class ContextFactory;
};
class HTTPClientFactory;

class FWPlatform : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };
  
 FWPlatform(float _display_scale, const char * _glsl_version, bool _has_es3)
   : display_scale(_display_scale), glsl_version(_glsl_version), has_es3(_has_es3) {
    initialize(this);
  }

  const std::string & getGLSLVersion() const { return glsl_version; }
  
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual double getTime() const = 0;
  virtual std::shared_ptr<canvas::ContextFactory> createContextFactory() const = 0;
  virtual std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
  virtual void storeValue(const std::string & key, const std::string & value) = 0;
  virtual std::string loadValue(const std::string & key) = 0;
  
  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }
  
  FWApplication & getApplication() {
    auto ptr = getFirstChild();
    FWApplication * app = dynamic_cast<FWApplication*>(ptr.get());
    return *app;
  }
  
#ifdef HAS_SOUNDCANVAS
  SoundCanvas & getSoundCanvas() {
    if (soundCanvas == 0){
      soundCanvas = createSoundCanvas();
    }
    return *soundCanvas;
  }
#endif
  Logger & getLogger() {
    if (logger == 0) {
      logger = createLogger();
    }
    return *logger;
  }

  void setActiveView(int id) {
    activeViewId = id;
    Command c(Command::REQUEST_REDRAW, id);
    sendCommand(c);
  }
  
  void sendCommand(const Command & command) override {
    if (command.getType() == Command::SHOW_VIEW) {
      setActiveView(command.getInternalId());
    } else if (!activeViewId && (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_NATIVE_OPENGL_VIEW)) {
      setActiveView(command.getChildInternalId());
    }
  }

  void postEvent(int internal_id, EventBase & ev) {
    Element * e = 0;
    if (!internal_id) {
      auto ptr = getFirstChild();
      e = ptr.get();
    } else {
      e = getElementByInternalId(internal_id);
    }
    if (e) ev.dispatch(*e);
    else {
      std::ostringstream s;
      s << "Failed to dispatch event " << typeid(ev).name() << " id: " << internal_id;
      getLogger().println(s.str());
    }
    if (ev.isRedrawNeeded()) {
      Command c(Command::REQUEST_REDRAW, internal_id);
      sendCommand(c);
    }
  }

  const std::shared_ptr<PrimitiveRenderer> & getRenderer() { return renderer; }
  void setRenderer(const std::shared_ptr<PrimitiveRenderer> & _renderer) { renderer = _renderer; }
    
  int getNextInternalId() { return nextInternalId++; }
  
  void setDisplayWidth(int w) { display_width = w; }
  void setDisplayHeight(int h) { display_height = h; }

  int getDisplayWidth() const { return display_width; }
  int getDisplayHeight() const { return display_height; }
  float getDisplayScale() const { return display_scale; }
  bool hasES3() const { return has_es3; }
  int getActiveViewId() const { return activeViewId; }

  virtual void createFBO(int flags) { }
  virtual std::string showTextEntryDialog(const std::string & message) = 0;

  int getModalResultValue() const { return modal_result_value; }
  const std::string & getModalResultText() const { return modal_result_text; }

  void exit() {
    Command c(Command::QUIT_APP, internal_id);
    sendCommand(c);
  }
  
 protected:
#ifdef HAS_SOUNDCANVAS
  virtual std::shared_ptr<SoundCanvas> createSoundCanvas() const {
    return std::make_shared<DummySoundCanvas>();
  }
#endif
  virtual std::shared_ptr<Logger> createLogger() const {
    return std::make_shared<BasicLogger>();
  }
  
  int display_width = 0, display_height = 0;
  float display_scale = 1.0f;
  std::string glsl_version;
  bool has_es3 = false;
  FWPreferences preferences;
  int modal_result_value = 0;
  std::string modal_result_text;
    
 private:
#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> soundCanvas;
#endif
  std::shared_ptr<Logger> logger;
  std::shared_ptr<PrimitiveRenderer> renderer;
  int nextInternalId = 1;
  int activeViewId = 0;
};

#endif
