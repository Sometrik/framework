#ifndef _APPRUNNER_H_
#define _APPRUNNER_H_

#include <Runnable.h>
#include <PlatformThread.h>

class AppRunner : public Runnable {
 public:
  AppRunner() { }
  
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

#endif
