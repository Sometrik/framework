#ifndef _BASICPLATFORM_H_
#define _BASICPLATFORM_H_

#include <PosixThread.h>
#include <CurlClient.h>
#include <Logger.h>

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
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
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

      for (auto & ev : evs) {
      	Element::postEventToElement(ev.first, *ev.second.get());
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
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
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

#endif
