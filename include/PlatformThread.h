#ifndef _PLATFORMTHREAD_H_
#define _PLATFORMTHREAD_H_

#include <EventHandler.h>
#include <Runnable.h>
#include <SysEvent.h>
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

class PlatformThread : public EventHandler {
 public:
  enum FileType {
    NORMAL = 1,
    DATABASE,
    CACHE_DATABASE
  };

 PlatformThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : application(_application), runnable(_runnable)
    {   
  }

#if 0
  ~PlatformThread() {
    Element::unregisterElement(this);    
  }
#endif

  void setParentThread(std::shared_ptr<PlatformThread> & _parent_thread) {
    parent_thread = _parent_thread;
    if (_parent_thread.get()) {
      setActualDisplayWidth(_parent_thread->getActualDisplayWidth());
      setActualDisplayHeight(_parent_thread->getActualDisplayHeight());
      setDisplayScale(_parent_thread->getDisplayScale());
    }
  }
  
  virtual bool startThread(std::shared_ptr<PlatformThread> thread) = 0;
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
  virtual bool testDestroy() = 0;
  virtual std::unique_ptr<Logger> createLogger(const std::string & name) const = 0;
  virtual std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) = 0;
    
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

  FWApplication & getApplication() { return *application; }
  const FWApplication & getApplication() const { return *application; }

  void postEvent(int target_internal_id, const Event & ev) {
    auto & app = getApplication();
    if (target_internal_id == 0) target_internal_id = app.getInternalId();
    auto t = app.getThreadPtr();
    if (t) t->sendEvent(target_internal_id, ev);
  }
  void postEvent(const Event & ev) { postEvent(0, ev); }

#if 0
  void exitApp() {
    Command c(Command::QUIT_APP, getInternalId());
    sendCommand(c);
  }
#endif

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

  void onSysEvent(SysEvent & ev) override {
    // Element::onSysEvent(ev);

    if (ev.getType() == SysEvent::TERMINATE_THREAD) {
      setDestroyed();
      ev.setHandled(true);
    } else if (ev.getType() == SysEvent::THREAD_TERMINATED) {
      auto it = subthreads.find(ev.getThreadId());
      if (it != subthreads.end()) {
	subthreads.erase(it);
      }
      if (exit_when_threads_terminated && subthreads.empty()) {
	terminate();        
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

  static std::shared_ptr<PlatformThread> run(std::shared_ptr<PlatformThread> currentThread, std::shared_ptr<Runnable> runnable) {
    if (currentThread.get()) {
      auto thread = currentThread->createThread(runnable);
      thread->setParentThread(currentThread);
      currentThread->subthreads[thread->getInternalId()] = thread;
      if (currentThread->startThread(thread)) {
	return thread;
      }
      currentThread->subthreads.erase(thread->getInternalId());
    }
    return std::shared_ptr<PlatformThread>();
  }

 protected:
  virtual void setDestroyed() = 0;
  virtual void initializeThread() { }  
  virtual void deinitializeThread() { }

  std::shared_ptr<PlatformThread> getParentThread() { return parent_thread.lock(); }

  void closeSubthreads() {
    while (!subthreads.empty()) {
      auto evs = pollEvents(true);
      for (auto & ed : evs) {
        ed.second->dispatch(*this);
      }
    }
  }
  void finalize() {
    SysEvent ev(SysEvent::THREAD_TERMINATED);
    ev.setThreadId(getInternalId());
    ev.setRunnable(runnable.get());

    auto pt = getParentThread();
    if (pt) {
      pt->sendEvent(pt->getInternalId(), ev);
    }
    postEvent(0, ev);
  }
  
  static void startThread2(std::shared_ptr<PlatformThread> thread) {
    thread->initializeThread();
    if (thread->runnable.get()) {
      thread->runnable->start(thread);
    }
    thread->closeSubthreads();
    thread->deinitializeThread();
    thread->finalize();
  }

  bool exit_when_threads_terminated = false;

  std::shared_ptr<FWApplication> application;

 private:
  std::weak_ptr<PlatformThread> parent_thread;
  int actual_display_width = 0, actual_display_height = 0;
  float display_scale = 1.0f;
  std::unordered_map<int, std::shared_ptr<PlatformThread> > subthreads;
  std::shared_ptr<Runnable> runnable;
  int modal_result_value = 0;
  bool batchOpen = false;
  std::vector<Command> commandBatch;
};

#endif
