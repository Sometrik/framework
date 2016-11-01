#include "FWPlatformBase.h"
#include "FWContextBase.h"

#ifndef __ANDROID__
#include <EGL/egl.h>
#endif

#include <cstring>
#include <iostream>

using namespace std;

#ifndef __APPLE__
///
// GetContextRenderableType()
//
//    Check whether EGL_KHR_create_context extension is supported.  If so,
//    return EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT
//
static EGLint GetContextRenderableType( EGLDisplay eglDisplay ) {
#ifdef EGL_KHR_create_context
  const char *extensions = eglQueryString ( eglDisplay, EGL_EXTENSIONS );
  
  // check whether EGL_KHR_create_context is in the extension string
  if ( extensions != NULL && strstr( extensions, "EGL_KHR_create_context" ) ) {
    // extension is supported
    return EGL_OPENGL_ES3_BIT_KHR;
  }
#endif
  // extension is not supported
  return EGL_OPENGL_ES2_BIT;
}
#endif

bool
FWPlatformBase::createContext(FWContextBase * context, const char * title, int requested_width, int requested_height) {
#ifndef __APPLE__
  EGLConfig config;
  EGLint majorVersion;
  EGLint minorVersion;
  EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
  
#ifdef ANDROID
  // For Android, get the width/height from the window rather than what the
  // application requested.
  unsigned int width = ANativeWindow_getWidth ( eglNativeWindow );
  unsigned int height = ANativeWindow_getHeight ( eglNativeWindow );
#else
  unsigned int width = requested_width;
  unsigned int height = requested_height;
#endif
#endif

#if 0
  unsigned int actual_width = (unsigned int)(width * getDisplayScale());
  unsigned int actual_height = (unsigned int)(height * getDisplayScale());
  context->setWindowSize(width, height, actual_width, actual_height);
#endif
  
  if (!createWindow( context, title)) {
    return false;
  }
  
#ifndef __APPLE__
  cerr << "creating display: " << eglNativeDisplay << endl;
  
  eglDisplay = eglGetDisplay( eglNativeDisplay );
  if ( eglDisplay == EGL_NO_DISPLAY ) {
    return false;
  }
  
  // Initialize EGL
  if ( !eglInitialize ( eglDisplay, &majorVersion, &minorVersion ) ) {
    return false;
  }

  unsigned int flags = 0; // fix
  
  {
    EGLint numConfigs = 0;
    EGLint attribList[] =
      {
	EGL_RED_SIZE,       5,
	EGL_GREEN_SIZE,     6,
	EGL_BLUE_SIZE,      5,
	EGL_ALPHA_SIZE,     EGL_DONT_CARE,
	EGL_DEPTH_SIZE,     ( flags & FBO_DEPTH ) ? 24 : EGL_DONT_CARE,
	EGL_STENCIL_SIZE,   ( flags & FBO_STENCIL ) ? 8 : EGL_DONT_CARE,
	// EGL_SAMPLE_BUFFERS, ( flags & ES_WINDOW_MULTISAMPLE ) ? 1 : 0,
	// if EGL_KHR_create_context extension is supported, then we will use
	// EGL_OPENGL_ES3_BIT_KHR instead of EGL_OPENGL_ES2_BIT in the attribute list
	EGL_RENDERABLE_TYPE, GetContextRenderableType ( eglDisplay ),
	EGL_NONE
      };
    
    // Choose config
    if ( !eglChooseConfig ( eglDisplay, attribList, &config, 1, &numConfigs ) )
      {
	return false;
      }
    
    if ( numConfigs < 1 )
      {
	return false;
      }
  }
  
#ifdef ANDROID
  // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using ANativeWindow_setBuffersGeometry
  {
    EGLint format = 0;
    eglGetConfigAttrib ( eglDisplay, config, EGL_NATIVE_VISUAL_ID, &format );
    ANativeWindow_setBuffersGeometry ( eglNativeWindow, 0, 0, format );
  }
#endif // ANDROID
  
  // Create a surface
  eglSurface = eglCreateWindowSurface ( eglDisplay, config,
					eglNativeWindow, NULL );
  
  if (eglSurface == EGL_NO_SURFACE) {
    return false;
  }
  
  // Create a GL context
  eglContext = eglCreateContext ( eglDisplay, config,
				  EGL_NO_CONTEXT, contextAttribs );
  
  if (eglContext == EGL_NO_CONTEXT) {
    return false;
  }
  
  // Make the context current
  if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
    return false;
  }

  // context->onResize(width, height);

#endif // #ifndef __APPLE__
  
  return true;
}
