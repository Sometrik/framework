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
