#ifndef _GL_H_
#define _GL_H_

#if defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined __ANDROID__
#include <importgl.h>
#else
#define GL_GLEXT_PROTOTYPES

#ifdef WIN32
#include <GL/glew.h>
#include <windows.h>
#endif

#include <GL/gl.h>

#ifdef _WIN32
#include "../system/win32/glext.h"
#else
#include <GL/glext.h>
#endif
#endif

#endif

#if defined __APPLE__ || defined __ANDROID__
#ifndef GL_RED
#define GL_RED 0
#endif
#ifndef GL_R8
#define GL_R8 0
#endif
#ifndef GL_R32F
#define GL_R32F 0
#endif
#ifndef GL_RG8
#define GL_RG8 0
#endif
#ifndef GL_RG
#define GL_RG 0
#endif
#ifndef GL_RGB8
#define GL_RGB8 0
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2 0
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0
#endif
#ifndef GL_COMPRESSED_RED_RGTC1
#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#endif
#ifndef GL_COMPRESSED_RG_RGTC2
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#endif
#endif


#endif
