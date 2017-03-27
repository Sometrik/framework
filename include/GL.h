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

#if defined __APPLE__ || defined __ANDROID__
#ifndef GL_RED
#define GL_RED 0x8D94
#endif
#ifndef GL_R8
#define GL_R8 0x8229
#endif
#ifndef GL_R32F
#define GL_R32F 0x8815
#endif
#ifndef GL_RG8
#define GL_RG8 0x822B
#endif
#ifndef GL_RG
#define GL_RG 0x8227
#endif
#ifndef GL_RGB8
#define GL_RGB8 0x8051
#endif
#ifndef GL_RGBA8
#define GL_RGBA8 0x8058
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
#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT 1
#endif
#ifndef GL_INT_2_10_10_10_REV
#define GL_INT_2_10_10_10_REV 0
#endif
#endif


#endif
