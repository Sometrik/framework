#ifndef _IOSMAINTHREAD_H_
#define _IOSMAINTHREAD_H_

#import "ViewController.h"

#include <iOSCFClient.h>
#include <ContextQuartz2D.h>
#include <PosixThread.h>

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

class iOSAppRunner : public Runnable {
 public:
  iOSAppRunner() { }
  
  void run() override {
    auto thread = getThreadPtr();
    if (thread.get()) {
      auto & app = thread->getApplication();
      app.initialize(thread);

      thread->startEventLoop();
#if 0
      deinitializeRenderer();
#endif
      Command c(Command::QUIT_APP, getInternalId());
      thread->sendCommand(c);
    }
  }  
};

class iOSMainThread : public PosixThread {
public:
  iOSMainThread(std::shared_ptr<FWApplication> _application, std::shared_ptr<Runnable> _runnable);

  void startEventLoop() override;

  void sendCommands(const std::vector<Command> & commands) override;
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<iOSCFClientFactory>(new iOSCFClientFactory);
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
  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override;
  void setSurface(int internal_id, canvas::Surface & surface) override;
  int startModal() override;
  void endModal(int value) override;
  void sendHeartbeat() override { }
      
  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override;
  
  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new iOSLogger(name));
  }

  void startDebugMode();
  // bool back();
  void sendIntValue(int viewId, int value);
  void sendTextValue(int viewId, const std::string & value);
  void sendVisibilityEvent(int viewId, bool visibility);
  void sendCommandEvent(int viewId, int elementId);
  void sendScrollChangedEvent(int viewId, int scrollPos, int scrollRem, int height);
  void sendTimerEvent(int timerId);
  void sendImageRequest(int viewId, unsigned int width, unsigned int height, const std::string & url, int internalFormat = 0);
  void cancelImageRequest(int viewId);
    
  void handleEventFromThread(int target_element_id, Event * event);
    
  ViewController * viewController = 0;

  int exit_loop = 0;
  bool isDestroyed = false;
  bool canDraw = false;
};

#endif


