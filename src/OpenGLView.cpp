#include <OpenGLView.h>
#include <FWPlatform.h>
#include <Message.h>
#include <Logger.h>

#include <sstream>

#include <GL.h>

using namespace std;

void
OpenGLView::initialize(FWPlatform * _platform) {
  FWViewBase::initialize(_platform);
  sendMessage(Message(Message::CREATE_OPENGL_VIEW, getParentInternalId(), getInternalId()));
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
