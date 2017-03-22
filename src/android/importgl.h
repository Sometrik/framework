#ifndef IMPORTGL_H_INCLUDED
#define IMPORTGL_H_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif


#include <GLES2/gl2.h>
#ifndef __ANDROID__
#include <jni.h>
#endif /* !__ANDROID__ */

/* Dynamically fetches pointers to the egl & gl functions.
 * Should be called once on application initialization.
 * Returns non-zero on success and 0 on failure.
 */
extern int importGLInit();

/* Frees the handle to egl & gl functions library.
 */
extern void importGLDeinit();

/* Use DISABLE_IMPORTGL if you want to link the OpenGL ES at
 * compile/link time and not import it dynamically runtime.
 */
#ifndef DISABLE_IMPORTGL


#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT


#ifndef __ANDROID__
FNDEF(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
FNDEF(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
FNDEF(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
FNDEF(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
FNDEF(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value));
FNDEF(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLDisplay, eglGetDisplay, (NativeDisplayType display));
FNDEF(EGLint, eglGetError, (void));
FNDEF(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
FNDEF(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
FNDEF(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface draw));
FNDEF(EGLBoolean, eglTerminate, (EGLDisplay dpy));
#endif /* !__ANDROID__ */

FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glClear, (GLbitfield mask));
//FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDisableClientState, (GLenum array));
FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableClientState, (GLenum array));
FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
FNDEF(GLenum, glGetError, (void));
FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
FNDEF(void, glLoadIdentity, (void));
FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
FNDEF(void, glMatrixMode, (GLenum mode));
FNDEF(void, glMultMatrixx, (const GLfixed *m));
FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glPopMatrix, (void));
FNDEF(void, glPushMatrix, (void));
FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glShadeModel, (GLenum mode));
FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
FNDEF(void, glAttachShader, (GLuint program, GLuint shader));
FNDEF(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar *name));
FNDEF(void, glActiveTexture, (GLenum texture));
FNDEF(void, glBindBuffer, (GLenum target, GLuint buffer));
FNDEF(void, glBindTexture, (GLenum target,  GLuint texture));
FNDEF(void, glBufferData, (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage));
FNDEF(void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha));
FNDEF(void, glCompileShader, (GLuint shader));
FNDEF(void, glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data));
FNDEF(void, glCompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid * data));
FNDEF(GLuint, glCreateProgram, (void));
FNDEF(GLuint, glCreateShader, (GLenum shaderType));
FNDEF(void, glDeleteTextures, (GLsizei n, const GLuint * textures));
FNDEF(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const GLvoid * indices));
FNDEF(void, glDisableVertexAttribArray, (GLuint index));
FNDEF(void, glEnableVertexAttribArray, (GLuint index));
FNDEF(void, glGenBuffers, (GLsizei n, GLuint * buffers));
FNDEF(void, glDepthMask, (GLboolean flag));
FNDEF(void, glDetachShader, (GLuint program, GLuint shader));
FNDEF(void, glGetProgramInfoLog, (GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog));
FNDEF(void, glGetProgramiv, (GLuint program, GLenum pname, GLint *params));
FNDEF(void, glGetShaderInfoLog, (GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog));
FNDEF(void, glGenerateMipmap, (GLenum target));
FNDEF(void, glGenTextures, (GLsizei n, GLuint * textures));
FNDEF(void, glGetIntegerv, (GLenum pname, GLint * params));
FNDEF(const GLubyte*, glGetString, (GLenum name));
FNDEF(void, glLineWidth, (GLfloat width));
FNDEF(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
FNDEF(void, glDeleteBuffers, (GLsizei n, const GLuint * buffers));
FNDEF(void, glDeleteProgram, (GLuint program));
FNDEF(void, glDeleteShader, (GLuint shader));
FNDEF(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint *params));
FNDEF(GLint, glGetUniformLocation, (GLuint program, const GLchar *name));
FNDEF(void, glLinkProgram, (GLuint program));
FNDEF(void, glPixelStorei, (GLenum pname, GLint param));
FNDEF(void, glPolygonOffset, (GLfloat factor, GLfloat units));
FNDEF(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar * const *string, const GLint *length));
FNDEF(void, glStencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask));
FNDEF(void, glStencilMask, (GLuint mask));
FNDEF(void, glStencilOpSeparate, (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass));
FNDEF(void, glUseProgram, (GLuint program));
FNDEF(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer));
FNDEF(void, glTexImage2D, (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid * data));
FNDEF(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param));
FNDEF(void, glTexParameteri, (GLenum target, GLenum pname, GLint param));
FNDEF(void, glTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid * pixels));
FNDEF(void, glUniform1f, (GLint location, GLfloat v0));
FNDEF(void, glUniform1i, (GLint location, GLint v0));
FNDEF(void, glUniform2iv, (GLint location, GLsizei count, const GLint *value));
FNDEF(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat *value));
FNDEF(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat *value));
FNDEF(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value));

//GL 3
FNDEF(void, glBindVertexArray, (GLuint array));
FNDEF(void, glInvalidateFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum *attachments));
FNDEF(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays));
FNDEF(void, glDrawArraysInstanced, (GLenum mode, GLint first, GLsizei count, GLsizei primcount));
FNDEF(void, glGenVertexArrays, (GLsizei n, GLuint *arrays));
FNDEF(void, glVertexAttribDivisor, (GLuint index, GLuint divisor));
FNDEF(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height));



#undef FN
#define FNPTR(name) funcPtr_##name
#ifndef IMPORTGL_NO_FNPTR_DEFS

// Redirect egl* and gl* function calls to funcPtr_egl* and funcPtr_gl*.


#ifndef __ANDROID__
#define eglChooseConfig         FNPTR(eglChooseConfig)
#define eglCreateContext        FNPTR(eglCreateContext)
#define eglCreateWindowSurface  FNPTR(eglCreateWindowSurface)
#define eglDestroyContext       FNPTR(eglDestroyContext)
#define eglDestroySurface       FNPTR(eglDestroySurface)
#define eglGetConfigAttrib      FNPTR(eglGetConfigAttrib)
#define eglGetConfigs           FNPTR(eglGetConfigs)
#define eglGetDisplay           FNPTR(eglGetDisplay)
#define eglGetError             FNPTR(eglGetError)
#define eglInitialize           FNPTR(eglInitialize)
#define eglMakeCurrent          FNPTR(eglMakeCurrent)
#define eglSwapBuffers          FNPTR(eglSwapBuffers)
#define eglTerminate            FNPTR(eglTerminate)
#endif /* !__ANDROID__ */

#define glBlendFunc             FNPTR(glBlendFunc)
#define glClear                 FNPTR(glClear)
//#define glClearColorx           FNPTR(glClearColorx)
#define glColor4x               FNPTR(glColor4x)
#define glColorPointer          FNPTR(glColorPointer)
#define glDisable               FNPTR(glDisable)
#define glDisableClientState    FNPTR(glDisableClientState)
#define glDrawArrays            FNPTR(glDrawArrays)
#define glEnable                FNPTR(glEnable)
#define glEnableClientState     FNPTR(glEnableClientState)
#define glFrustumx              FNPTR(glFrustumx)
#define glGetError              FNPTR(glGetError)
#define glLightxv               FNPTR(glLightxv)
#define glLoadIdentity          FNPTR(glLoadIdentity)
#define glMaterialx             FNPTR(glMaterialx)
#define glMaterialxv            FNPTR(glMaterialxv)
#define glMatrixMode            FNPTR(glMatrixMode)
#define glMultMatrixx           FNPTR(glMultMatrixx)
#define glNormalPointer         FNPTR(glNormalPointer)
#define glPopMatrix             FNPTR(glPopMatrix)
#define glPushMatrix            FNPTR(glPushMatrix)
#define glRotatex               FNPTR(glRotatex)
#define glScalex                FNPTR(glScalex)
#define glShadeModel            FNPTR(glShadeModel)
#define glTranslatex            FNPTR(glTranslatex)
#define glVertexPointer         FNPTR(glVertexPointer)
#define glViewport              FNPTR(glViewport)
#define glAttachShader              FNPTR(glAttachShader)
#define glBindAttribLocation              FNPTR(glBindAttribLocation)
#define glActiveTexture              FNPTR(glActiveTexture)
#define glBindBuffer              FNPTR(glBindBuffer)
#define glBindTexture              FNPTR(glBindTexture)
#define glBufferData              FNPTR(glBufferData)
#define glColorMask              FNPTR(glColorMask)
#define glCompileShader              FNPTR(glCompileShader)
#define glCompressedTexImage2D              FNPTR(glCompressedTexImage2D)
#define glCompressedTexSubImage2D              FNPTR(glCompressedTexSubImage2D)
#define glCreateProgram              FNPTR(glCreateProgram)
#define glCreateShader              FNPTR(glCreateShader)
#define glDeleteTextures              FNPTR(glDeleteTextures)
#define glDrawElements              FNPTR(glDrawElements)
#define glDisableVertexAttribArray              FNPTR(glDisableVertexAttribArray)
#define glEnableVertexAttribArray              FNPTR(glEnableVertexAttribArray)
#define glGenBuffers              FNPTR(glGenBuffers)
#define glDepthMask              FNPTR(glDepthMask)
#define glDetachShader              FNPTR(glDetachShader)
#define glGetProgramInfoLog              FNPTR(glGetProgramInfoLog)
#define glGetProgramiv              FNPTR(glGetProgramiv)
#define glGetShaderInfoLog              FNPTR(glGetShaderInfoLog)
#define glGenerateMipmap              FNPTR(glGenerateMipmap)
#define glGenTextures              FNPTR(glGenTextures)
#define glGetIntegerv              FNPTR(glGetIntegerv)
#define glGetString              FNPTR(glGetString)
#define glLineWidth              FNPTR(glLineWidth)
#define glClearColor              FNPTR(glClearColor)
#define glDeleteBuffers              FNPTR(glDeleteBuffers)
#define glDeleteProgram              FNPTR(glDeleteProgram)
#define glDeleteShader              FNPTR(glDeleteShader)
#define glGetShaderiv              FNPTR(glGetShaderiv)
#define glGetUniformLocation              FNPTR(glGetUniformLocation)
#define glLinkProgram              FNPTR(glLinkProgram)
#define glPixelStorei              FNPTR(glPixelStorei)
#define glPolygonOffset              FNPTR(glPolygonOffset)
#define glShaderSource              FNPTR(glShaderSource)
#define glStencilFuncSeparate              FNPTR(glStencilFuncSeparate)
#define glStencilMask              FNPTR(glStencilMask)
#define glStencilOpSeparate              FNPTR(glStencilOpSeparate)
#define glUniform1f              FNPTR(glUniform1f)
#define glUniform1i              FNPTR(glUniform1i)
#define glUniform2iv              FNPTR(glUniform2iv)
#define glUniform3fv              FNPTR(glUniform3fv)
#define glUniform4fv              FNPTR(glUniform4fv)
#define glUniformMatrix4fv              FNPTR(glUniformMatrix4fv)
#define glUseProgram              FNPTR(glUseProgram)
#define glVertexAttribPointer              FNPTR(glVertexAttribPointer)
#define glTexImage2D              FNPTR(glTexImage2D)
#define glTexParameterf              FNPTR(glTexParameterf)
#define glTexSubImage2D              FNPTR(glTexSubImage2D)
#define glTexParameteri              FNPTR(glTexParameteri)

//GL 3
#define glBindVertexArray       FNPTR(glBindVertexArray)
#define glInvalidateFramebuffer       FNPTR(glInvalidateFramebuffer)
#define glDeleteVertexArrays       FNPTR(glDeleteVertexArrays)
#define glDrawArraysInstanced       FNPTR(glDrawArraysInstanced)
#define glGenVertexArrays       FNPTR(glGenVertexArrays)
#define glVertexAttribDivisor       FNPTR(glVertexAttribDivisor)
#define glTexStorage2D       FNPTR(glTexStorage2D)

#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#ifdef __cplusplus
}
#endif


#endif // !IMPORTGL_H_INCLUDED
