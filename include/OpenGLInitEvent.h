#ifndef _OPENGLINITEVENT_H_
#define _OPENGLINITEVENT_H_

#include <Event.h>

class OpenGLInitEvent : public Event {
public:
 OpenGLInitEvent(double _timestamp, int _opengl_version, bool _is_opengl_es)
   : Event(_timestamp), opengl_version(_opengl_version), is_opengl_es(_is_opengl_es) { }

  Event * dup() const override { return new OpenGLInitEvent(*this); }
  void dispatch(EventHandler & element) override;

  int getOpenGLVersion() const { return opengl_version; }
  bool isES() const { return is_opengl_es; }
  
 private:
  int opengl_version; // 0x30000 = OpenGL es 3.0, 0x20000 = OpenGL ES 2.0
  bool is_opengl_es;
};

#endif
