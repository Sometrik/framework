#ifndef _OPENGLINITEVENT_H_
#define _OPENGLINITEVENT_H_

#include <EventBase.h>

class OpenGLInitEvent : public EventBase {
public:
 OpenGLInitEvent(double _timestamp, int _opengl_es_version)
   : EventBase(_timestamp), opengl_es_version(_opengl_es_version) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<OpenGLInitEvent>(*this); }
  void dispatch(Element & element) override;

  int getOpenGLESVersion() const { return opengl_es_version; }
  
 private:
  int opengl_es_version; // 0x30000 = OpenGL es 3.0, 0x20000 = OpenGL ES 2.0
};

#endif
