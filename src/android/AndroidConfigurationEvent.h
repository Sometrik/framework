#ifndef _ANDROIDCONFIGURATIONEVENT_H_
#define _ANDROIDCONFIGURATIONEVENT_H_

#include <ConfigurationEvent.h>
#include <android/native_window.h>

class AndroidConfigurationEvent : public ConfigurationEvent {
public:
 AndroidConfigurationEvent(double _timestamp, ANativeWindow * _window)
   : ConfigurationEvent(_timestamp), window(_window) { }

  ANativeWindow * getWindow() { return window; }
  std::shared_ptr<EventBase> dup() const { return  std::make_shared<AndroidConfigurationEvent>(*this); }
  void dispatch(Element & element);
  
 private:
  ANativeWindow * window = 0;
};

#endif
