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
 BasicThread(int _id, PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PosixThread(_id, _parent_thread, _platform, _runnable) { }

#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    auto & platform = getPlatform();
    return std::make_shared<BasicThread>(platform.getNextThreadId(), this, &platform, runnable);
  }

  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
  }

  int sendCommand(const Command & command) override {
    return 0;
  }
};

class BasicMainThread : public PlatformThread {
public:
 BasicMainThread(int _id, FWPlatform * _platform, std::shared_ptr<Runnable> & _runnable)
   : PlatformThread(_id, 0, _platform, _runnable)
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
    auto & platform = getPlatform();
    return std::make_shared<BasicThread>(platform.getNextThreadId(), this, &platform, runnable);
  }

  void sleep(double t) override {
    usleep((unsigned int)(t * 1000000));
  }

  int sendCommand(const Command & command) override {
    return 0;    
  }

  bool start() override { return false; }
  bool testDestroy() override { return false; }
  void terminate() override { }
};

class BasicPlatform : public FWPlatform {
 public:
 BasicPlatform() { }

  std::string getBundleFilename(const char * filename) override {
    return filename;
  }
  std::string getLocalFilename(const char * filename, FileType type) override {
    return filename;
  }
  void pushEvent(int internal_id, const Event & ev) override {

  }
  std::string loadTextAsset(const char * filename) override {
    return "";
  }
  int startModal() override { return 0; }
  void endModal() override { }
};

#endif
