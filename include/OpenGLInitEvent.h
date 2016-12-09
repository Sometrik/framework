#ifndef _OPENGLINITEVENT_H_
#define _OPENGLINITEVENT_H_

#include <Event.h>

class OpenGLInitEvent : public Event {
public:
 OpenGLInitEvent(double _timestamp, int _opengl_es_version)
   : Event(_timestamp), opengl_es_version(_opengl_es_version) { }

  Event * dup() const override { return new OpenGLInitEvent(*this); }
  void dispatch(EventHandler & element) override;

  int getOpenGLESVersion() const { return opengl_es_version; }
  
 private:
  int opengl_es_version; // 0x30000 = OpenGL es 3.0, 0x20000 = OpenGL ES 2.0
};

#endif
