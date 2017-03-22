#define LINUX

#include <stdlib.h>
#include <dlfcn.h>
static void *sGLESSO = NULL;

#define IMPORTGL_NO_FNPTR_DEFS
#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL
#include "importgl.h"


/* Imports function pointers to selected function calls in OpenGL ES 2 or 3
 * The function pointers are stored as global symbols with equivalent function
 * name but prefixed with "funcPtr_". Standard gl/egl calls are redirected to
 * the function pointers with preprocessor macros (see importgl.h).
 */
int importGLInit()
{
    int result = 1;

//    sGLESSO = dlopen("libGLESv3_CM.so", RTLD_NOW);
    sGLESSO = dlopen("libGLESv3.so", RTLD_NOW);
    int es3 = 0;
    if (sGLESSO == NULL) {
      es3 = 1;
      sGLESSO = dlopen("libGLESv2.so", RTLD_NOW);
      if (sGLESSO == NULL) {
        return 0;   // Cannot find OpenGL ES Common or Common Lite SO.
      }
    }

    if (sGLESSO == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite SO.

#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)dlsym(sGLESSO, #funcName); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)

  IMPORT_FUNC(glBlendFunc);
  IMPORT_FUNC(glClear);
  IMPORT_FUNC(glDisable);
  IMPORT_FUNC(glDrawArrays);
  IMPORT_FUNC(glEnable);
  IMPORT_FUNC(glGetError);
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

    return result;
}


void importGLDeinit()
{
  dlclose(sGLESSO);
}
