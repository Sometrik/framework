#ifndef _ANDROIDCONFIGURATIONEVENT_H_
#define _ANDROIDCONFIGURATIONEVENT_H_

#include <ConfigurationEvent.h>

class AndroidConfigurationEvent : public ConfigurationEvent {
public:
 AndroidConfigurationEvent(double _timestamp, ANativeWindow * _window)
   : ConfigurationEvent(_timestamp), window(_window) { }

  ANativeWindow * getWindow() { return window; }
  
 private:
  ANativeWindow * window = 0;
};

#endif
