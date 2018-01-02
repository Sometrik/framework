#ifndef _BASICPLATFORM_H_
#define _BASICPLATFORM_H_

#include <PosixThread.h>
#include <CurlClient.h>

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
 BasicThread(PlatformThread * _parent_thread, std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : PosixThread(_parent_thread,_application, _runnable) { }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return std::make_shared<BasicThread>(this, application, runnable);
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

  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override { }
  void setSurface(int internal_id, canvas::Surface & surface) override { }
  int startModal() override { return 0; }
  void endModal(int value) override { }

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }
};

class BasicMainThread : public PlatformThread {
public:
 BasicMainThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(0, _application, _runnable)
  {
    
  }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return std::make_shared<BasicThread>(this, application, runnable);
  }

  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
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

  bool start() override { return false; }
  bool testDestroy() override { return false; }

  void sendEvent(int internal_id, const Event & ev) override {
  }

  void startEventLoop() override {

  }

  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool block) override {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    return r;
  }

  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override { }
  void setSurface(int internal_id, canvas::Surface & surface) override { }
  int startModal() override { return 0; }
  void endModal(int value) override { }

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new BasicLogger(name));
  }

 protected:
  void setDestroyed() override { }
};

#endif
