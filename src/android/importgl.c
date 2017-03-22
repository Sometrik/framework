#undef WIN32
#undef LINUX
#ifdef _MSC_VER
// Desktop or mobile Win32 environment:
#define WIN32
#else
// Linux environment:
#define LINUX
#endif

#ifndef DISABLE_IMPORTGL

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
static HMODULE sGLESDLL = NULL;
#endif // WIN32

#ifdef LINUX
#include <stdlib.h>
#include <dlfcn.h>
static void *sGLESSO = NULL;
#endif // LINUX

#endif /* DISABLE_IMPORTGL */

#define IMPORTGL_NO_FNPTR_DEFS
#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL
#include "importgl.h"


/* Imports function pointers to selected function calls in OpenGL ES Common
 * or Common Lite profile DLL or shared object. The function pointers are
 * stored as global symbols with equivalent function name but prefixed with
 * "funcPtr_". Standard gl/egl calls are redirected to the function pointers
 * with preprocessor macros (see importgl.h).
 */
int importGLInit()
{
    int result = 1;

#ifndef DISABLE_IMPORTGL

#undef IMPORT_FUNC

#ifdef WIN32
    sGLESDLL = LoadLibrary(_T("lGLESv2"));
    if (sGLESDLL == NULL)
        sGLESDLL = LoadLibrary(_T("lGLESv2"));
    if (sGLESDLL == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite DLL.

    /* The following fetches address to each egl & gl function call
     * and stores it to the related function pointer. Casting through
     * void * results in warnings with VC warning level 4, which
     * could be fixed by casting to the true type for each fetch.
     */
#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)GetProcAddress(sGLESDLL, _T(#funcName)); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // WIN32

#ifdef LINUX
#ifdef __ANDROID__
//    sGLESSO = dlopen("libGLESv3_CM.so", RTLD_NOW);
//    sGLESSO = dlopen("libGLESv3.so", RTLD_NOW);
    sGLESSO = dlopen("libGLESv3", RTLD_NOW);
#else /* !__ANDROID__ */
    sGLESSO = dlopen("libGLES_CM.so", RTLD_NOW);
    if (sGLESSO == NULL)
        sGLESSO = dlopen("libGLES_CL.so", RTLD_NOW);
#endif /* !__ANDROID__ */
    if (sGLESSO == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite SO.

#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // LINUX

#ifndef __ANDROID__
  IMPORT_FUNC(eglChooseConfig);
  IMPORT_FUNC(eglCreateContext);
  IMPORT_FUNC(eglCreateWindowSurface);
  IMPORT_FUNC(eglDestroyContext);
  IMPORT_FUNC(eglDestroySurface);
  IMPORT_FUNC(eglGetConfigAttrib);
  IMPORT_FUNC(eglGetConfigs);
  IMPORT_FUNC(eglGetDisplay);
  IMPORT_FUNC(eglGetError);
  IMPORT_FUNC(eglInitialize);
  IMPORT_FUNC(eglMakeCurrent);
  IMPORT_FUNC(eglSwapBuffers);
  IMPORT_FUNC(eglTerminate);
#endif /* !__ANDROID__ */

  IMPORT_FUNC(glBlendFunc);
  IMPORT_FUNC(glClear);
//  IMPORT_FUNC(glClearColorx);
  IMPORT_FUNC(glColor4x);
  IMPORT_FUNC(glColorPointer);
  IMPORT_FUNC(glDisable);
  IMPORT_FUNC(glDisableClientState);
  IMPORT_FUNC(glDrawArrays);
  IMPORT_FUNC(glEnable);
  IMPORT_FUNC(glEnableClientState);
  IMPORT_FUNC(glFrustumx);
  IMPORT_FUNC(glGetError);
  IMPORT_FUNC(glLightxv);
  IMPORT_FUNC(glLoadIdentity);
  IMPORT_FUNC(glMaterialx);
  IMPORT_FUNC(glMaterialxv);
  IMPORT_FUNC(glMatrixMode);
  IMPORT_FUNC(glMultMatrixx);
  IMPORT_FUNC(glNormalPointer);
  IMPORT_FUNC(glPopMatrix);
  IMPORT_FUNC(glPushMatrix);
  IMPORT_FUNC(glRotatex);
  IMPORT_FUNC(glScalex);
  IMPORT_FUNC(glShadeModel);
  IMPORT_FUNC(glTranslatex);
  IMPORT_FUNC(glVertexPointer);
  IMPORT_FUNC(glViewport);
  IMPORT_FUNC(glAttachShader);
  IMPORT_FUNC(glBindAttribLocation);
  IMPORT_FUNC(glActiveTexture);
  IMPORT_FUNC(glBindBuffer);
  IMPORT_FUNC(glBindTexture);
  IMPORT_FUNC(glBufferData);
  IMPORT_FUNC(glColorMask);
  IMPORT_FUNC(glCompileShader);
  IMPORT_FUNC(glCompressedTexImage2D);
  IMPORT_FUNC(glCompressedTexSubImage2D);
  IMPORT_FUNC(glCreateProgram);
  IMPORT_FUNC(glCreateShader);
  IMPORT_FUNC(glDeleteTextures);
  IMPORT_FUNC(glDrawElements);
  IMPORT_FUNC(glDisableVertexAttribArray);
  IMPORT_FUNC(glEnableVertexAttribArray);
  IMPORT_FUNC(glGenBuffers);
  IMPORT_FUNC(glDepthMask);
  IMPORT_FUNC(glDetachShader);
  IMPORT_FUNC(glGetProgramInfoLog);
  IMPORT_FUNC(glGetProgramiv);
  IMPORT_FUNC(glGetShaderInfoLog);
  IMPORT_FUNC(glGenerateMipmap);
  IMPORT_FUNC(glGenTextures);
  IMPORT_FUNC(glGetIntegerv);
  IMPORT_FUNC(glGetString);
  IMPORT_FUNC(glLineWidth);
  IMPORT_FUNC(glClearColor);
  IMPORT_FUNC(glDeleteBuffers);
  IMPORT_FUNC(glDeleteProgram);
  IMPORT_FUNC(glDeleteShader);
  IMPORT_FUNC(glGetShaderiv);
  IMPORT_FUNC(glGetUniformLocation);
  IMPORT_FUNC(glLinkProgram);
  IMPORT_FUNC(glPixelStorei);
  IMPORT_FUNC(glPolygonOffset);
  IMPORT_FUNC(glShaderSource);
  IMPORT_FUNC(glStencilFuncSeparate);
  IMPORT_FUNC(glStencilMask);
  IMPORT_FUNC(glStencilOpSeparate);
  IMPORT_FUNC(glUniform1f);
  IMPORT_FUNC(glUniform1i);
  IMPORT_FUNC(glUniform2iv);
  IMPORT_FUNC(glUniform3fv);
  IMPORT_FUNC(glUniform4fv);
  IMPORT_FUNC(glUniformMatrix4fv);
  IMPORT_FUNC(glUseProgram);
  IMPORT_FUNC(glVertexAttribPointer);
  IMPORT_FUNC(glTexImage2D);
  IMPORT_FUNC(glTexParameterf);
  IMPORT_FUNC(glTexSubImage2D);
  IMPORT_FUNC(glTexParameteri);

  //GL 3
  IMPORT_FUNC(glBindVertexArray);
  IMPORT_FUNC(glInvalidateFramebuffer);
  IMPORT_FUNC(glDeleteVertexArrays);
  IMPORT_FUNC(glDrawArraysInstanced);
  IMPORT_FUNC(glGenVertexArrays);
  IMPORT_FUNC(glVertexAttribDivisor);
  IMPORT_FUNC(glTexStorage2D);

#endif /* DISABLE_IMPORTGL */

    return result;
}


void importGLDeinit()
{
#ifndef DISABLE_IMPORTGL
#ifdef WIN32
    FreeLibrary(sGLESDLL);
#endif

#ifdef LINUX
    dlclose(sGLESSO);
#endif
#endif /* DISABLE_IMPORTGL */
}