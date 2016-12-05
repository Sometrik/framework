#ifndef _OPENGLINITEVENT_H_
#define _OPENGLINITEVENT_H_

#include <EventBase.h>

class OpenGLInitEvent : public EventBase {
public:
 OpenGLInitEvent(double _timestamp, int _opengl_es_version)
   : EventBase(_timestamp), opengl_es_version(_opengl_es_version) { }

  int getOpenGLESVersion() const { return opengl_es_version; }
  
 private:
  int opengl_es_version; // 300 = OpenGL es 3.0
};

#endif
