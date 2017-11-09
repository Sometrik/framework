#include <OpenGLView.h>
#include <FWPlatform.h>
#include <Command.h>
#include <Logger.h>
#include <ResizeEvent.h>
#include <FWApplication.h>

#include <OpenGLInitEvent.h>

#include <GL.h>

using namespace std;

void
OpenGLView::create() {
  Command c(Command::CREATE_OPENGL_VIEW, getParentInternalId(), getInternalId());
  c.setTextValue(getTitle());
  sendCommand(c);
  
  logical_width = int(platform->getActualDisplayWidth() / platform->getDisplayScale());
  logical_height = int(platform->getActualDisplayHeight() / platform->getDisplayScale());
  actual_width = platform->getActualDisplayWidth();
  actual_height = platform->getActualDisplayHeight();
}

void
OpenGLView::onResizeEvent(ResizeEvent & ev) {
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
    
    getApplication().getLogger().println("got OpenGL error " + to_string((int)err));
  }
}
