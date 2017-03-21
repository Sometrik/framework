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

FNDEF(void, glBindVertexArray, (GLuint array));
FNDEF(void, glInvalidateFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum *attachments));
FNDEF(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays));
FNDEF(void, glDrawArraysInstanced, (GLenum mode, GLint first, GLsizei count, GLsizei primcount));
FNDEF(void, glGenVertexArrays, (GLsizei n, GLuint *arrays));
FNDEF(void, glVertexAttribDivisor, (GLuint index, GLuint divisor));
//This should propably be something different
FNDEF(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height));



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
//This should propably be something different
#define glTexStorage2D       FNPTR(glTexStorage2D)

#endif // !IMPORTGL_NO_FNPTR_DEFS


#endif // !DISABLE_IMPORTGL


#ifdef __cplusplus
}
#endif


#endif // !IMPORTGL_H_INCLUDED
