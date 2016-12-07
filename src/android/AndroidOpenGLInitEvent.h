#ifndef _ANDROIDOPENGLINITEVENT_H_
#define _ANDROIDOPENGLINITEVENT_H_

#include <OpenGLInitEvent.h>
#include <android/native_window.h>

class AndroidOpenGLInitEvent : public OpenGLInitEvent {
public:
 AndroidOpenGLInitEvent(double _timestamp, int _opengl_es_version, ANativeWindow * _window)
   : OpenGLInitEvent(_timestamp, _opengl_es_version), window(_window) { }

  ANativeWindow * getWindow() { return window; }
  std::shared_ptr<EventBase> dup() const { return  std::make_shared<AndroidOpenGLInitEvent>(*this); }
  
 private:
  ANativeWindow * window = 0;
};

#endif
