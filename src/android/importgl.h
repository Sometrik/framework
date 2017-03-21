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

//#ifndef __ANDROID__
//FNDEF(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
//FNDEF(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
//FNDEF(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
//FNDEF(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
//FNDEF(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
//FNDEF(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value));
//FNDEF(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config));
//FNDEF(EGLDisplay, eglGetDisplay, (NativeDisplayType display));
//FNDEF(EGLint, eglGetError, (void));
//FNDEF(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
//FNDEF(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
//FNDEF(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface draw));
//FNDEF(EGLBoolean, eglTerminate, (EGLDisplay dpy));
//#endif /* !__ANDROID__ */
//
//FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
//FNDEF(void, glClear, (GLbitfield mask));
////FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
//FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
//FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
//FNDEF(void, glDisable, (GLenum cap));
//FNDEF(void, glDisableClientState, (GLenum array));
//FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
//FNDEF(void, glEnable, (GLenum cap));
//FNDEF(void, glEnableClientState, (GLenum array));
//FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
//FNDEF(GLenum, glGetError, (void));
//FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
//FNDEF(void, glLoadIdentity, (void));
//FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
//FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
//FNDEF(void, glMatrixMode, (GLenum mode));
//FNDEF(void, glMultMatrixx, (const GLfixed *m));
//FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
//FNDEF(void, glPopMatrix, (void));
//FNDEF(void, glPushMatrix, (void));
//FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
//FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
//FNDEF(void, glShadeModel, (GLenum mode));
//FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
//FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
//FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
//FNDEF(void, glBindBuffer, (GLenum target, GLuint buffer));
FNDEF(void, glBindVertexArray, (GLuint array));
FNDEF(void, glInvalidateFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum *attachments));
FNDEF(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays));
FNDEF(void, glDrawArraysInstanced, (GLenum mode, GLint first, GLsizei count, GLsizei primcount));
FNDEF(void, glGenVertexArrays, (GLsizei n, GLuint *arrays));
FNDEF(void, glVertexAttribDivisor, (GLuint index, GLuint divisor));
FNDEF(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height));
FNDEF(void, glBindTexture, (GLenum target, GLuint texture));



#undef FN
#define FNPTR(name) funcPtr_##name
#ifndef IMPORTGL_NO_FNPTR_DEFS

// Redirect egl* and gl* function calls to funcPtr_egl* and funcPtr_gl*.

#define glBindVertexArray       FNPTR(glBindVertexArray)
#define glInvalidateFramebuffer       FNPTR(glInvalidateFramebuffer)
#define glDeleteVertexArrays       FNPTR(glDeleteVertexArrays)
#define glDrawArraysInstanced       FNPTR(glDrawArraysInstanced)
#define glGenVertexArrays       FNPTR(glGenVertexArrays)
#define glVertexAttribDivisor       FNPTR(glVertexAttribDivisor)
#define glTexStorage2D       FNPTR(glTexStorage2D)
#define glBindTexture       FNPTR(glBindTexture)

#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#ifdef __cplusplus
}
#endif


#endif // !IMPORTGL_H_INCLUDED
