#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <Element.h>
#include <Event.h>
#include <FWPreferences.h>
#include <MobileAccount.h>
#include <Mutex.h>

#ifdef HAS_SOUNDCANVAS
#include <SoundCanvas.h>
#endif

#include <memory>
#include <unordered_map>
#include <atomic>

class Runnable;
class PlatformThread;

class FWPlatform : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };
  
  FWPlatform() { }

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

  void onSysEvent(SysEvent & ev) override;

  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }
  
#ifdef HAS_SOUNDCANVAS
  SoundCanvas & getSoundCanvas() {
    if (soundCanvas == 0){
      soundCanvas = createSoundCanvas();
    }
    return *soundCanvas;
  }
#endif
    
  void postEvent(int internal_id, Event & ev);
      
  int getModalResultValue() const { return modal_result_value; }
  const std::string & getModalResultText() const { return modal_result_text; }

  bool run(std::shared_ptr<Runnable> runnable);

  void terminateThreads();
 
  MobileAccount & getMobileAccount();

  void dumpThreads() const;

  void registerElement(Element * e) {
    registered_elements[e->getInternalId()] = e;
  }

  void unregisterElement(Element * e) {
    registered_elements.erase(e->getInternalId());
  }

  const PlatformThread * getThreadById(int id) const {
    auto it = threads.find(id);
    if (it != threads.end()) {
      return it->second.get();
    } else {
      return 0;
    }    
  }
  
  void create() override;

 protected:
  Element * getRegisteredElement(int internal_id) {
    auto it = registered_elements.find(internal_id);
    return it != registered_elements.end() ? it->second : 0;
  }
  
#ifdef HAS_SOUNDCANVAS
  virtual std::shared_ptr<SoundCanvas> createSoundCanvas() const {
    return std::make_shared<DummySoundCanvas>();
  }
#endif

  size_t getNumRunningThreads() const {
    MutexLocker m(mutex);
    return threads.size();
  }

  int getNextThreadId() { return next_thread_id.fetch_add(1); }

  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;

  FWPreferences preferences;
  int modal_result_value = 0;
  std::string modal_result_text;
  bool exit_when_threads_terminated = false;
    
 private:
#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> soundCanvas;
#endif
  std::unordered_map<int, std::shared_ptr<PlatformThread> > threads;
  std::unordered_map<int, Element *> registered_elements;
  mutable Mutex mutex;

  static std::atomic<int> next_thread_id;
};

#endif
