#ifndef _IOSMAINTHREAD_H_
#define _IOSMAINTHREAD_H_

#include "ViewController.h"

#include <iOSClient.h>
#include <ContextQuartz2D.h>
#include <PlatformThread.h>

#include "iOSLogger.h"

#include <FilenameConverter.h>

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

class iOSMainThread : public PlatformThread {
public:
  iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable);
    
  bool start() override { return true; }
  bool testDestroy() override { return false; }
  void sendCommands(const std::vector<Command> & commands) override;
  void sleep(double t) override;
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<iOSClientFactory>(new iOSClientFactory);
  }
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::Quartz2DContextFactory>(new canvas::Quartz2DContextFactory(getDisplayScale(), new canvas::BundleFilenameConverter));
  }
  std::string loadTextAsset(const char * filename) override;
  std::string getBundleFilename(const char * filename) override {
    return "";
  }
  std::string getLocalFilename(const char * filename, FileType type) override {
    return "";
  }
  void sendEvent(int internal_id, const Event & ev) override;
  std::vector<std::pair<int, std::shared_ptr<Event> > > pollEvents(bool blocking = false) override {
    std::vector<std::pair<int, std::shared_ptr<Event> > > r;
    return r;
  }
  void startEventLoop() override { }
  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override;
  void setSurface(int internal_id, canvas::Surface & surface) override;
  int startModal() override;
  void endModal(int value) override;
  void sendHeartbeat() override { }
      
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override;
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new iOSLogger(name));
  }

  void sendMemoryWarning();
  void startDebugMode();
  bool back();
  void sendIntValue(int viewId, int value);
  void sendTextValue(int viewId, const std::string & value);
  void sendTimerEvent(int timerId);
  void sendImageRequest(int viewId, unsigned int width, unsigned int height, const std::string & url, int internalFormat = 0);
    
  void handleEventFromThread(int target_element_id, Event * event);
    
  ViewController * viewController = 0;
  NSUserDefaults * defaults = 0;
  
 protected:
  void setDestroyed() override { }
  
};

#endif


