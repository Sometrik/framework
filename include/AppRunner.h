#ifndef _APPRUNNER_H_
#define _APPRUNNER_H_

#include <Runnable.h>
#include <PlatformThread.h>

class AppRunner : public Runnable {
 public:
  AppRunner() { }
  
  void run() override {
    auto t = getThreadPtr();
    if (t.get()) {
      auto & app = t->getApplication();
      app.initialize(t);

      t->startEventLoop();
#if 0
      deinitializeRenderer();
#endif
      Command c(Command::QUIT_APP, getInternalId());
      t->sendCommand(c);
    }
  }  
};

#endif
