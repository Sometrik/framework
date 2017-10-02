#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <Element.h>
#include <Event.h>
#include <Mutex.h>

#ifdef HAS_SOUNDCANVAS
#include <SoundCanvas.h>
#endif

#include <memory>
#include <unordered_map>
#include <atomic>

class FWPlatform : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };
  
  FWPlatform() : next_thread_id(1) { }

  bool isA(const std::string & className) const override {
    if (className == "FWPlatform") return true;
    return Element::isA(className);
  }
  
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual std::string loadTextAsset(const char * filename) = 0; 
  virtual void pushEvent(int internal_id, const Event & ev) = 0;
  virtual int startModal() = 0;
  virtual void endModal() = 0;

  virtual void createFBO(int flags) { }

#if 0
  void onSysEvent(SysEvent & ev) override;
#endif

  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }
  
#ifdef HAS_SOUNDCANVAS
  SoundCanvas & getSoundCanvas() {
    if (soundCanvas == 0){
      soundCanvas = createSoundCanvas();
    }
    return *soundCanvas;
  }
#endif
    
  void postEvent(int internal_id, Event & ev) {
    auto e = getRegisteredElement(internal_id);
    if (e) {
      ev.dispatch(*e);
    }
  }
      
  int getModalResultValue() const { return modal_result_value; }
  const std::string & getModalResultText() const { return modal_result_text; }
 
  void registerElement(Element * e) {
    MutexLocker m(mutex);
    registered_elements[e->getInternalId()] = e;
  }

  void unregisterElement(Element * e) {
    MutexLocker m(mutex);
    registered_elements.erase(e->getInternalId());
  }

  void create() override {
    sendCommand(Command(Command::CREATE_PLATFORM, getParentInternalId(), getInternalId()));
  }

  int getNextThreadId() { return next_thread_id.fetch_add(1); }

 protected:
  Element * getRegisteredElement(int internal_id) {
    MutexLocker m(mutex);
    auto it = registered_elements.find(internal_id);
    return it != registered_elements.end() ? it->second : 0;
  }
  
#ifdef HAS_SOUNDCANVAS
  virtual std::shared_ptr<SoundCanvas> createSoundCanvas() const {
    return std::make_shared<DummySoundCanvas>();
  }
#endif

  int modal_result_value = 0;
  std::string modal_result_text;
    
 private:
#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> soundCanvas;
#endif
  std::unordered_map<int, Element *> registered_elements;

  std::atomic<int> next_thread_id;

  Mutex mutex;
};

#endif
