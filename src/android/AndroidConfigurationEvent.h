#ifndef _ANDROIDCONFIGURATIONEVENT_H_
#define _ANDROIDCONFIGURATIONEVENT_H_

#include <ConfigurationEvent.h>
#include <android/native_window.h>

class AndroidConfigurationEvent : public ConfigurationEvent {
public:
 AndroidConfigurationEvent(double _timestamp, int _opengl_es_version, ANativeWindow * _window)
   : ConfigurationEvent(_timestamp, _opengl_es_version), window(_window) { }

  ANativeWindow * getWindow() { return window; }
  std::shared_ptr<EventBase> dup() const { return  std::make_shared<AndroidConfigurationEvent>(*this); }
  void dispatch(Element & element);
  
 private:
  ANativeWindow * window = 0;
};

#endif
