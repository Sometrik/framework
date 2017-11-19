#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <Element.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <FWPlatform.h>
#include <FWApplication.h>

#include <memory>

class Event;
class HTTPClientFactory;
class Logger;

namespace canvas {
  class ContextFactory;
  class PackedImageData;
  class Surface;
};

class PlatformThread : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };

 PlatformThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : application(_application), platform(_platform), parent_thread(_parent_thread), runnable(_runnable)
    {
    initialize(this);
    if (_parent_thread) {
      setActualDisplayWidth(_parent_thread->getActualDisplayWidth());
      setActualDisplayHeight(_parent_thread->getActualDisplayHeight());
      setDisplayScale(_parent_thread->getDisplayScale());
    }
  }

  ~PlatformThread() {
    auto & platform = getPlatform();
    platform.unregisterElement(this);    
  }

  bool isA(const std::string & className) const override {
    if (className == "PlatformThread") return true;
    else return Element::isA(className);
  }  
  
  virtual bool start() = 0;
  virtual bool testDestroy() = 0;
  virtual void sendCommands(const std::vector<Command> & commands) = 0;
  virtual void sleep(double t) = 0;
  virtual std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
#ifndef NO_CANVAS
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
#endif
  virtual std::string loadTextAsset(const char * filename) = 0; 
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual void sendEvent(int internal_id, const Event & ev) = 0;
  virtual std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool blocking = false) = 0;
  virtual void startEventLoop() = 0;
  virtual void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) = 0;
  virtual void setSurface(int internal_id, canvas::Surface & surface) = 0;
  virtual int startModal() = 0;
  virtual void endModal(int value) = 0;
  virtual void sendHeartbeat() { }
      
  bool terminate() {
    sendEvent(getInternalId(), SysEvent(SysEvent::TERMINATE_THREAD));
    
    terminateChildren();
    exit_when_threads_terminated = true;

    return getNumRunningThreads() == 0;
  }
  
  void terminateChildren() {
    for (auto & thread : subthreads) {
      thread.second->terminate();
    }
  }

  std::string getBundleFilename(const std::string & filename) { return getBundleFilename(filename.c_str()); }

  Runnable & getRunnable() { return *runnable; }
  Runnable * getRunnablePtr() { return runnable.get(); }
  const Runnable * getRunnablePtr() const { return runnable.get(); }

  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }

  FWApplication & getApplication() { return *application; }
  const FWApplication & getApplication() const { return *application; }

  void postEvent(int internal_id, const Event & ev) {
    if (internal_id == 0) internal_id = getApplication().getInternalId();
    getApplication().getThread().sendEvent(internal_id, ev);
  }
  void postEvent(const Event & ev) { postEvent(0, ev); }

  std::shared_ptr<PlatformThread> run(std::shared_ptr<Runnable> runnable) {
    auto thread = createThread(runnable);
    subthreads[thread->getInternalId()] = thread;
    if (thread->start()) {
      return thread;
    } else {
      subthreads.erase(thread->getInternalId());
      return std::shared_ptr<PlatformThread>();
    }
  }
  
  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const = 0;
  
  void exitApp() {
    Command c(Command::QUIT_APP, getInternalId());
    sendCommand(c);
  }

  void setActualDisplayWidth(int w) { actual_display_width = w; }
  void setActualDisplayHeight(int h) { actual_display_height = h; }
  void setDisplayScale(float f) { display_scale = f; }

  int getActualDisplayWidth() const { return actual_display_width; }
  int getActualDisplayHeight() const { return actual_display_height; }
  float getDisplayScale() const { return display_scale; }

  const PlatformThread * getThreadById(int id) const {
    auto it = subthreads.find(id);
    if (it != subthreads.end()) {
      return it->second.get();
    } else {
      return 0;
    }
  }

  size_t getNumRunningThreads() const {
    return subthreads.size();
  }

  bool hasRunningThreads() const {
    return !subthreads.empty();
  }

  const std::unordered_map<int, std::shared_ptr<PlatformThread> > & getSubThreads() const {
    return subthreads;
  }

  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;

  void onSysEvent(SysEvent & ev) override {
    Element::onSysEvent(ev);

    if (ev.getType() == SysEvent::TERMINATE_THREAD) {
      setDestroyed();
      ev.setHandled(true);
    } else if (ev.getType() == SysEvent::THREAD_TERMINATED) {
      auto it = subthreads.find(ev.getThreadId());
      if (it != subthreads.end()) {
	subthreads.erase(it);
      }
      if (exit_when_threads_terminated && subthreads.empty()) {
        if (parent_thread) {
          terminate();
        } else {
          exitApp();
        }
      }
    }
  }

  void setModalResultValue(int v) { modal_result_value = v; }
  int getModalResultValue() const { return modal_result_value; }
  
  void beginBatch() {
    batchOpen = true;
  }

  void commitBatch() {
    batchOpen = false;
    if (!commandBatch.empty()) {
      sendCommands(commandBatch);
      commandBatch.clear();
    }
  }

  void sendCommand(const Command & command) {
    commandBatch.push_back(command);
    if (!isInTransaction()) {
      sendCommands(commandBatch);
      commandBatch.clear();
    }
  }
  
  bool isInTransaction() const { return batchOpen; }

 protected:
  void create() override { }

  virtual void setDestroyed() = 0;
  virtual void initializeThread() { }  
  virtual void deinitializeThread() { }
  virtual void startRunnable() {
    if (runnable.get()) {
      runnable->start(this);
    }
  }  
  void start2() {
    initializeThread();
    startRunnable();

    while (!subthreads.empty()) {
      auto evs = pollEvents(true);
      for (auto & ed : evs) {
        ed.second->dispatch(*this);
      }
    }

    deinitializeThread();

    SysEvent ev(SysEvent::THREAD_TERMINATED);
    ev.setThreadId(getInternalId());
    ev.setRunnable(runnable.get());

    if (parent_thread) {
      parent_thread->sendEvent(parent_thread->getInternalId(), ev);
    }

    postEvent(0, ev);
  }

  bool exit_when_threads_terminated = false;

 protected:
  std::shared_ptr<FWApplication> application;

 private:
  FWPlatform * platform;
  PlatformThread * parent_thread;
  int actual_display_width = 0, actual_display_height = 0;
  float display_scale = 1.0f;
  std::unordered_map<int, std::shared_ptr<PlatformThread> > subthreads;
  std::shared_ptr<Runnable> runnable;
  int modal_result_value = 0;
  bool batchOpen = false;
  std::vector<Command> commandBatch;
};

#endif
