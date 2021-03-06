#ifndef _ANDROIDOPENGLINITEVENT_H_
#define _ANDROIDOPENGLINITEVENT_H_

#include <OpenGLInitEvent.h>
#include <android/native_window.h>

class AndroidOpenGLInitEvent : public OpenGLInitEvent {
public:
 AndroidOpenGLInitEvent(int _opengl_es_version, bool _is_opengl_es, ANativeWindow * _window)
   : OpenGLInitEvent(_opengl_es_version, _is_opengl_es),
    window(_window) { }

  ANativeWindow * getWindow() { return window; }
  Event * dup() const { return new AndroidOpenGLInitEvent(*this); }
  
 private:
  ANativeWindow * window = 0;
};

#endif
