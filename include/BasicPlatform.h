#ifndef _BASICPLATFORM_H_
#define _BASICPLATFORM_H_

#include <PosixThread.h>
#include <CurlClient.h>
#include <Logger.h>
#include <TimerEvent.h>

#ifndef NO_CANVAS
#include <Context.h>
#endif

#include <fstream>
#include <unistd.h>
#include <sstream>
#include <iostream>

class BasicLogger : public Logger {
 public:
  BasicLogger(const std::string & _name) : Logger(_name) { }
  
  void println(const char * s) override {
    std::cerr << s << std::endl;
  }
};

class BasicThread : public PosixThread {
 public:
 BasicThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : PosixThread(_application, _runnable) { }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<httpclient::HTTPClientFactory> createHTTPClientFactory() const override {
    return std::make_unique<httpclient::CurlClientFactory>();
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & r) override {
    return std::make_shared<BasicThread>(application, r);
  }

  void sendCommands(const std::vector<Command> & commands) {
  }

  std::string getLocalFilename(const char * fn, FileType type) override { return fn; }
  std::string getBundleFilename(const char * filename) override { return filename; }
  std::string loadTextAsset(const char * filename) override {
    std::ifstream t(getBundleFilename(filename));
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
  }

  void setImageData(int _internal_id, std::shared_ptr<canvas::PackedImageData> image) override { }
  void setSurface(int _internal_id, canvas::Surface & surface) override { }
  int startModal() override { return 0; }
  void endModal(int value) override { }

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }
};

class BasicMainThread : public PosixThread {
public:
 BasicMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable)
   : PosixThread(_application, _runnable)
  {
    
  }

  void startEventLoop() override {      
    while (getNumRunningThreads() != 0 || !testDestroy()) {
      auto evs = pollEvents();

      std::pair<int, std::shared_ptr<Event> > waiting_timer_event;
      for (auto & ed : evs) {
	auto internal_id = ed.first;
	auto & ev = ed.second;

	if (dynamic_cast<TimerEvent*>(ev.get())) {
	  waiting_timer_event = ed;
	} else if (internal_id == getInternalId()) { // thread is not an element
	  ev->dispatch(*this);
	} else if (!Element::postEventToElement(internal_id, *ev)) {
	  std::cerr << "unable to post Event\n";
	}
      }

      if (waiting_timer_event.second) {
	Element::postEventToElement(waiting_timer_event.first, *waiting_timer_event.second);
      }

#if 0
      if (prev_heartbeat_time + 10 <= DateTime::now() / 1000) {
        getApplication().showToast("Application is not responding: " + getApplication().getStatusText(), 9);
	sendHeartbeat();
      }
#endif
    }
  }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<httpclient::HTTPClientFactory> createHTTPClientFactory() const override {
    return std::make_unique<httpclient::CurlClientFactory>();
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & r) override {
    return std::make_shared<BasicThread>(application, r);
  }

  void sendCommands(const std::vector<Command> & commands) {
    for (auto & command : commands) {
      switch (command.getType()) {
      case Command::QUIT_APP:
	exit(0);
	break;
	
      default:
	break;
      }
    }
  }

  std::string getLocalFilename(const char * fn, FileType type) override { return fn; }
  std::string getBundleFilename(const char * filename) override { return filename; }
  std::string loadTextAsset(const char * filename) override {
    std::ifstream t(getBundleFilename(filename));
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
  }

  void setImageData(int _internal_id, std::shared_ptr<canvas::PackedImageData> image) override { }
  void setSurface(int _internal_id, canvas::Surface & surface) override { }
  int startModal() override { return 0; }
  void endModal(int value) override { }

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }
};

#endif
