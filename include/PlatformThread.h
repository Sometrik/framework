#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <Element.h>
#include <Logger.h>
#include <Runnable.h>
#include <SysEvent.h>
#include <FWPlatform.h>
#include <FWApplication.h>

#include <exception>
#include <memory>
#include <iostream>

class Event;
class HTTPClientFactory;
class Logger;

namespace canvas {
  class ContextFactory;
};

class PlatformThread : public Element {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };

 PlatformThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : platform(_platform), application(_application), parent_thread(_parent_thread), runnable(_runnable)
    {
    if (_parent_thread) {
      setActualDisplayWidth(_parent_thread->getActualDisplayWidth());
      setActualDisplayHeight(_parent_thread->getActualDisplayHeight());
      setDisplayScale(_parent_thread->getDisplayScale());
    }
  }

  bool isA(const std::string & className) const override {
    if (className == "PlatformThread") return true;
    else return Element::isA(className);
  }  
  
  virtual bool start() = 0;
  virtual bool testDestroy() = 0;
  virtual int sendCommand(const Command & command) = 0;
  virtual void sleep(double t) = 0;
  virtual std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const = 0;
#ifndef NO_CANVAS
  virtual std::unique_ptr<canvas::ContextFactory> createContextFactory() const = 0;
#endif
  virtual std::string loadTextAsset(const char * filename) = 0; 
  virtual std::string getBundleFilename(const char * filename) = 0;
  virtual std::string getLocalFilename(const char * filename, FileType type) = 0;
  virtual void sendEvent(int internal_id, const Event & ev) = 0;
  virtual std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents() = 0;
  virtual void startEventLoop() = 0;

  virtual bool terminate() {
    std::cerr << "terminating " << subthreads.size() << " threads:\n";
    dumpThreads();
    
    for (auto & thread : subthreads) {
      thread.second->terminate();
    }
    exit_when_threads_terminated = true;

    return getNumRunningThreads() == 0;
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
  
  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }

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

  void dumpThreads() const {
    std::cerr << "running threads:\n";
    for (auto & t : subthreads) {
      auto & r = t.second->getRunnable();
      std::cerr << "  thread " << t.second->getInternalId() << "/" << typeid(r).name() << ": " << r.getStatusText() << std::endl;
    }
  }

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

  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;

  void onSysEvent(SysEvent & ev) override {
    Element::onSysEvent(ev);

    if (ev.getType() == SysEvent::THREAD_TERMINATED) {
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
    } else if (ev.getType() == SysEvent::END_MODAL) {
      setModalResultValue(ev.getValue());
      setModalResultText(ev.getTextValue());
    }
  }

  void setModalResultValue(int v) { modal_result_value = v; }
  void setModalResultText(const std::string & t) { modal_result_text = t; }
  
  int getModalResultValue() const { return modal_result_value; }
  const std::string & getModalResultText() const { return modal_result_text; }

 protected:
  virtual void initialize() { }  
  virtual void deinitialize() { }
  virtual void startRunnable() {
    try {
      getRunnable().start(this);
    } catch (std::exception & e) {
      auto logger = createLogger("PlatformThread");
      logger->println("PlatformThread: Runnable threw an exception: " + std::string(e.what()));
    }
  }
  void start2() {
    initialize();
    startRunnable();
    deinitialize();

    if (parent_thread) {
      SysEvent ev(SysEvent::THREAD_TERMINATED);
      ev.setThreadId(getInternalId());
      ev.setRunnable(runnable.get());
      parent_thread->sendEvent(parent_thread->getInternalId(), ev);
    }
  }

  bool exit_when_threads_terminated = false;

 protected:
  std::shared_ptr<FWApplication> application;

 private:
  int id;
  FWPlatform * platform;
  PlatformThread * parent_thread;
  int actual_display_width = 0, actual_display_height = 0;
  float display_scale = 1.0f;
  std::unordered_map<int, std::shared_ptr<PlatformThread> > subthreads;
  std::shared_ptr<Runnable> runnable;
  int modal_result_value = 0;
  std::string modal_result_text;
};

#endif
