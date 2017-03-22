#ifndef _GL_H_
#define _GL_H_

#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined GL_ES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <importgl.h>
#else
//#define GL_GLEXT_PROTOTYPES

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#endif

#ifdef __ANDROID__
#include <importgl.h>
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
