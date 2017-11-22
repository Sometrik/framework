#ifndef _IOSMAINTHREAD_H_
#define _IOSMAINTHREAD_H_

#include <PlatformThread.h>

class iOSMainThread : public PlatformThread {
public:
    iOSMainThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<Runnable> & _runnable) : PlatformThread(0, _application, _runnable) { }
    
    
};
#endif


