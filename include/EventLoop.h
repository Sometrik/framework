#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

class FWContextBase;

class EventLoop {
 public:
  EventLoop(FWContextBase * _application) : application(_application) { }
  virtual ~EventLoop() { }
  
  virtual void run() = 0;
  void stop() { keep_running = false; }

  FWContextBase * getApplication() { return application; }
  
 protected:
  bool doKeepRunning() const { return keep_running; }
      
 private:
  FWContextBase * application;
  bool keep_running = true;
};

#endif
