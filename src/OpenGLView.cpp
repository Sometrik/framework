#include <OpenGLView.h>
#include <FWPlatformBase.h>
#include <Message.h>
#include <Logger.h>

#include <sstream>

#ifdef __APPLE__
#include <OpenGLES/ES3/gl.h>
#else
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

using namespace std;

void
OpenGLView::initialize(FWPlatformBase * _platform) {
  FWViewBase::initialize(_platform);
  sendMessage(Message(Message::CREATE_OPENGL_VIEW, getId(), getParentId()));
  logical_width = int(_platform->getDisplayWidth() / _platform->getDisplayScale());
  logical_height = int(_platform->getDisplayHeight() / _platform->getDisplayScale());
  actual_width = _platform->getDisplayWidth();
  actual_height = _platform->getDisplayHeight();    
}

void
OpenGLView::checkGLError() {
  GLenum errLast = GL_NO_ERROR;
  for (;;) {
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
      return;
    }

    // normally the error is reset by the call to glGetError() but if
    // glGetError() itself returns an error, we risk looping forever here
    // so check that we get a different error than the last time
    if (err == errLast) {
      getPlatform().getLogger().println("OpenGL error state couldn't be reset.");
      return;
    }

    errLast = err;

    ostringstream s;
    s << "got OpenGL error " << err;
    getPlatform().getLogger().println(s.str());
  }
}
