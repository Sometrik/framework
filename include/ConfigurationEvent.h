#ifndef _CONFIGURATIONEVENT_H_
#define _CONFIGURATIONEVENT_H_

#include <EventBase.h>

class ConfigurationEvent : public EventBase {
public:
 ConfigurationEvent(double _timestamp, int _opengl_es_version)
   : EventBase(_timestamp), opengl_es_version(_opengl_es_version) { }

  int getOpenGLESVersion() const { return opengl_es_version; }
  
 private:
  int opengl_es_version; // 300 = OpenGL es 3.0
};

#endif
