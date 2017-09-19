#ifndef _BASICPLATFORM_H_
#define _BASICPLATFORM_H_

#include <FWPlatform.h>
#include <PosixThread.h>
#include <CurlClient.h>

#ifndef NO_CANVAS
#include <Context.h>
#endif

class BasicThread : public PosixThread {
 public:
 BasicThread(int _id, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PosixThread(_id, _platform, _runnable) { }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }
};

class DummyThread : public PlatformThread {
public:
 DummyThread(int _id, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(_id, _platform, _runnable)
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

  bool start() override { return false; }
  bool testDestroy() override { return false; }
  void terminate() override { }
};

class BasicPlatform : public FWPlatform {
 public:
 BasicPlatform() : FWPlatform(1.0f) { }

  std::string getBundleFilename(const char * filename) override {
    return filename;
  }
  std::string getLocalFilename(const char * filename, FileType type) override {
    return filename;
  }
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return std::make_shared<BasicThread>(getNextThreadId(), this, runnable);
  }
  void pushEvent(int internal_id, const Event & ev) override {

  }
  void sendCommand2(const Command & command) {
    
  }
  std::string loadTextAsset(const char * filename) {
    return "";
  }
};

#endif
