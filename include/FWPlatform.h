#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <Element.h>
#include <Event.h>
#include <Command.h>
#include <Logger.h>
#include <FWDefs.h>
#include <FWPreferences.h>
#include <MobileAccount.h>

#ifdef HAS_SOUNDCANVAS
#include <SoundCanvas.h>
#endif

#include <string>
#include <memory>
#include <list>

#ifndef NO_CANVAS
namespace canvas {
  class ContextFactory;
};
#endif

class HTTPClientFactory;
class Runnable;
class PlatformThread;

class FWPlatform : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };
  
  FWPlatform(float _display_scale);

  bool isA(const std::string & className) override {
    if (className == "FWPlatform") return true;
    return Element::isA(className);
  }
  
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
#ifndef NO_CANVAS
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
#endif
  virtual std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
  virtual void pushEvent(const Event & ev) = 0;
  virtual void setCursor(const std::string & cursor) { }

  void onSysEvent(SysEvent & ev) override;

  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }

  void storeValue(const std::string & key, const std::string & value) {
    Command c(Command::UPDATE_PREFERENCE, 0);
    c.setKey(key);
    c.setTextValue(value);
    sendCommand2(c);
  }
  
#ifdef HAS_SOUNDCANVAS
  SoundCanvas & getSoundCanvas() {
    if (soundCanvas == 0){
      soundCanvas = createSoundCanvas();
    }
    return *soundCanvas;
  }
#endif

  void setActiveViewId(int id) {
    activeViewId = id;
  }
  
  virtual void sendCommand2(const Command & command) = 0;
  
  void postEvent(int internal_id, Event & ev);
      
  void setDisplayWidth(int w) { display_width = w; }
  void setDisplayHeight(int h) { display_height = h; }

  int getDisplayWidth() const { return display_width; }
  int getDisplayHeight() const { return display_height; }
  float getDisplayScale() const { return display_scale; }
  int getActiveViewId() const { return activeViewId; }

  virtual void createFBO(int flags) { }

  int getModalResultValue() const { return modal_result_value; }
  const std::string & getModalResultText() const { return modal_result_text; }

  void exit() {
    Command c(Command::QUIT_APP, getInternalId());
    sendCommand2(c);
  }

  std::shared_ptr<PlatformThread> run(std::shared_ptr<Runnable> runnable);

  size_t getNumRunningThreads() const { return num_running_threads; }

  void terminateThreads();
  void disconnectThreads();

  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }

  virtual double getTime() const;

  void addToHistory(int view_internal_id) {
    view_back_history.push_back(view_internal_id);
    view_forward_history.clear();
  }

  int popViewBackHistory() {
    int id = 0;
    if (!view_back_history.empty()) {
      id = view_back_history.back();
      view_back_history.pop_back();
      view_forward_history.push_back(id);
    }
    return id;    
  }

  int popViewForwardHistory() {
    int id = 0;
    if (!view_forward_history.empty()) {
      id = view_forward_history.back();
      view_forward_history.pop_back();
      view_back_history.push_back(id);
    }
    return id;    
  }
  
  MobileAccount & getMobileAccount();

  Logger & getLogger() {
    if (!logger.get()) {
      logger = createLogger("Framework");
    }
    return *logger;
  }

  void dumpThreads() const;

 protected:

#ifdef HAS_SOUNDCANVAS
  virtual std::shared_ptr<SoundCanvas> createSoundCanvas() const {
    return std::make_shared<DummySoundCanvas>();
  }
#endif

  virtual std::shared_ptr<PlatformThread> run2(std::shared_ptr<Runnable> & runnable);

  int display_width = 0, display_height = 0;
  float display_scale = 1.0f;
  FWPreferences preferences;
  int modal_result_value = 0;
  std::string modal_result_text;
    
 private:
#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> soundCanvas;
#endif
  std::shared_ptr<Logger> logger;
  int activeViewId = 0;

  std::list<std::shared_ptr<PlatformThread> > threads;
  std::vector<int> view_back_history, view_forward_history;
  size_t num_running_threads = 0;
};

#endif
