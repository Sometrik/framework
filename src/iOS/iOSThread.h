#ifndef _IOSTHREAD_H_
#define _IOSTHREAD_H_

#include <PosixThread.h>

#include <iOSClient.h>
#include <FilenameConverter.h>
#include <ContextQuartz2D.h>

#include "iOSLogger.h"

namespace canvas {
  class BundleFilenameConverter : public FilenameConverter {
  public:
    bool convert(const std::string & input, std::string & output) {
      const char * filename = input.c_str();
      int n = (int)input.size();
      for (int i = n - 1; i >= 0; i--)  {
        if (filename[i] == '/')  {
          filename = filename + i + 1;
          break;
        }
      }
      
      // Given a fileName, convert into a path that can be used to open from the mainBundle
      NSString* fileNameNS = [NSString stringWithUTF8String:filename];
      NSString* baseName = [fileNameNS stringByDeletingPathExtension];
      NSString* extension = [fileNameNS pathExtension];
      NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
      const char * filename2 = [path cStringUsingEncoding:1];
      if (filename2) {
        output = filename2;
        return true;
      } else {
        return false;
      }
    }
  };
};

class iOSThread : public PosixThread {
public:
  iOSThread(PlatformThread * _parent_thread, std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_parent_thread, _application, _runnable) { }
  
  void sendCommands(const std::vector<Command> & commands) override {
    
  }
  
  void sleep(double t) override {
  
  }
  
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<iOSClientFactory>(new iOSClientFactory);
  }
  
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::Quartz2DContextFactory>(new canvas::Quartz2DContextFactory(getDisplayScale(), new canvas::BundleFilenameConverter));
  }
  
  std::string loadTextAsset(const char * filename) override {
    return "";
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
    return std::make_shared<iOSThread>(this, application, runnable);
  }
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new iOSLogger(name));
  }
};

#endif
