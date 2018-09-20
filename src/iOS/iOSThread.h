#ifndef _IOSTHREAD_H_
#define _IOSTHREAD_H_

#include <PosixThread.h>

class iOSThread : public PosixThread {
public:
  iOSThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_application, _runnable) { }
  
  void sendCommands(const std::vector<Command> & commands) override {
    auto pt = getParentThread();
    if (pt) pt->sendCommands(commands);
  }
  
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<iOSCFClientFactory>(new iOSCFClientFactory);
  }
  
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::Quartz2DContextFactory>(new canvas::Quartz2DContextFactory(getDisplayScale(), new canvas::BundleFilenameConverter));
  }
  
  std::string loadTextAsset(const char * filename) override {
    NSString * filename2 = [NSString stringWithUTF8String:filename];
    NSString * path = [[NSBundle mainBundle] pathForResource:filename2 ofType:nil];
    NSString * content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];
    std::string s = [content cStringUsingEncoding:NSUTF8StringEncoding];
    return s;
  }
  
  std::string getBundleFilename(const char * filename) override {
    return "";
  }
  
  std::string getLocalFilename(const char * filename, FileType type) override {
    return "";
  }
  
  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override {
    
  }
  
  void setSurface(int internal_id, canvas::Surface & surface) override {
    
  }
  
  int startModal() override { return 0; }
  void endModal(int value) override { }
  void sendHeartbeat() override { }
  
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return std::make_shared<iOSThread>(application, runnable);
  }
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new iOSLogger(name));
  }
};

#endif
