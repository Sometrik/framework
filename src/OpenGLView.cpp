#include <OpenGLView.h>
#include <FWPlatform.h>
#include <Command.h>
#include <Logger.h>
#include <ResizeEvent.h>
#include <FWApplication.h>

#include <OpenGLInitEvent.h>

#include <sstream>

#include <GL.h>

using namespace std;

void
OpenGLView::initialize(FWPlatform * _platform) {
  FWViewBase::initialize(_platform);
  sendCommand(Command(Command::CREATE_OPENGL_VIEW, getParentInternalId(), getInternalId()));
  logical_width = int(_platform->getDisplayWidth() / _platform->getDisplayScale());
  logical_height = int(_platform->getDisplayHeight() / _platform->getDisplayScale());
  actual_width = _platform->getDisplayWidth();
  actual_height = _platform->getDisplayHeight();
  initializeChildren();
}

void
OpenGLView::onResizeEvent(ResizeEvent & ev) {
  cerr << "OpenGLView: got resizeevent\n";
  logical_width = ev.getLogicalWidth();
  logical_height = ev.getLogicalHeight();
  actual_width = ev.getActualWidth();
  actual_height = ev.getActualHeight();
  ev.requestRedraw();
  ev.setHandled();
}

void
OpenGLView::onOpenGLInitEvent(OpenGLInitEvent & ev) {
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
      getApplication().getLogger().println("OpenGL error state couldn't be reset.");
      return;
    }

    errLast = err;

    ostringstream s;
    s << "got OpenGL error " << err;
    getApplication().getLogger().println(s.str());
  }
}
