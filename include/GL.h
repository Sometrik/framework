#ifndef _GL_H_
#define _GL_H_

#if defined __APPLE__
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif defined GL_ES
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#else
#define GL_GLEXT_PROTOTYPES

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#endif

#ifdef __ANDROID__
#include <GLES3/gl3.h>
#else
#include <GL/gl.h>

#ifdef _WIN32
#include "../system/win32/glext.h"
#else
#include <GL/glext.h>
#endif
#endif

#endif

#endif
