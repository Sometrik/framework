/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <string.h>
#include <android/log.h>
#include "Menu.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include <android/bitmap.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidPlatform.h>
#include <FWApplication.h>

#include <pthread.h>

#include <TouchEvent.h>
#include <SysEvent.h>
#include <TextEvent.h>
#include <CommandEvent.h>
#include <InitEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <AndroidConfigurationEvent.h>

#include <android_fopen.h>

using namespace std;

extern FWApplication * applicationMain();

std::string AndroidPlatform::getBundleFilename(const char * filename) {
  return filename;
}

std::string AndroidPlatform::getLocalFilename(const char * filename, FileType type) {

  auto env = getJNIEnv();
  jstring path;
  jstring jfilename;
  std::string result;
  switch (type) {
  case DATABASE:
  case CACHE_DATABASE:
    return "";
  case NORMAL:
    return "";
  }
  return "";
}

void
AndroidPlatform::sendCommand(const Command & command) {
  FWPlatform::sendCommand(command);

  auto env = getJNIEnv();
   int commandTypeId = int(command.getType());
   const char * textValue = command.getTextValue().c_str();
   const char * textValue2 = command.getTextValue2().c_str();
   jstring jtextValue = env->NewStringUTF(textValue);
   jstring jtextValue2 = env->NewStringUTF(textValue2);


   jobject jcommand = env->NewObject(javaCache.nativeCommandClass, javaCache.nativeCommandConstructor, framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2);

   //Fix these releases
//  env->ReleaseStringUTFChars(jtextValue, textValue);
//  env->ReleaseStringUTFChars(jtextValue2, textValue2);
  
#ifdef USE_NATIVE_SURFACE
  if (command.getType() == Command::SHOW_MESSAGE_DIALOG || command.getType() == Command::SHOW_INPUT_DIALOG) {
    renderLoop();
  }
#endif      
}

double
AndroidPlatform::getTime() const {
  auto env = getJNIEnv();
  double currentTime = (double)env->CallStaticLongMethod(javaCache.systemClass, javaCache.currentTimeMillisMethod);

  return currentTime;
}

bool
AndroidPlatform::initializeRenderer(ANativeWindow * _window) {
  window = _window;

  if (window) {
    const EGLint attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_BLUE_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_RED_SIZE, 8,
      EGL_NONE
    };
    EGLDisplay _display;
    EGLConfig _config;
    EGLint numConfigs;
    EGLint format;
    EGLSurface _surface;
    EGLContext _context;
    EGLint width;
    EGLint height;

    if ((_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
      //         LOG_ERROR("eglGetDisplay() returned error %d", eglGetError());
      return false;
    }
    if (!eglInitialize(_display, 0, 0)) {
      //         LOG_ERROR("eglInitialize() returned error %d", eglGetError());
      return false;
    }

    if (!eglChooseConfig(_display, attribs, &_config, 1, &numConfigs)) {
      //         LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format)) {
      //         LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    if (!(_surface = eglCreateWindowSurface(_display, _config, window, 0))) {
      //         LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!(_context = eglCreateContext(_display, _config, 0, 0))) {
      //         LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglMakeCurrent(_display, _surface, _surface, _context)) {
      //         LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglQuerySurface(_display, _surface, EGL_WIDTH, &width) ||
	!eglQuerySurface(_display, _surface, EGL_HEIGHT, &height)) {
      //         LOG_ERROR("eglQuerySurface() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    display = _display;
    surface = _surface;
    context = _context;

    getLogger().println("Piiiip");
    return true;
  } else {
    return false;
  }
}

void
AndroidPlatform::deinitializeRenderer() {
  if (display) {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    
    display = EGL_NO_DISPLAY;
    surface = EGL_NO_SURFACE;
    context = EGL_NO_CONTEXT;
  }

  if (window) {
    ANativeWindow_release(window);
  }
}

void
AndroidPlatform::startThread() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 1");
  pthread_create(&_threadId, 0, threadStartCallback, this);
}

void
AndroidPlatform::renderLoop() {
  runloop_level++;
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Looping louie");

  while (renderingEnabled) {
    if (!eventqueue.empty()) {
      auto ev = eventqueue.pop();
      postEvent(ev.first, *ev.second.get());

      auto ev2 = dynamic_cast<AndroidConfigurationEvent*>(ev.second.get());
      if (ev2) {
	initializeRenderer(ev2->getWindow());
      }
    }

    // process incoming messages
//    switch (_msg) {
//
//    case MSG_WINDOW_SET:
//      initialize();
//      break;
//
//    case MSG_RENDER_LOOP_EXIT:
//      renderingEnabled = false;
//      destroy();
//      break;
//
//    default:
//      break;
//    }
//    _msg = MSG_NONE;

  }

  getLogger().println("Looping Louie is out");
  runloop_level--;
}

void
AndroidPlatform::swapBuffers() {
  if (!eglSwapBuffers(display, surface)) {
    getLogger().println("error eglSwapBuffers");
  }
}

void* AndroidPlatform::threadStartCallback(void *myself) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 2");

  AndroidPlatform * aplatform = (AndroidPlatform*) myself;


  JNIEnv * env;
  aplatform->getJavaVM()->AttachCurrentThread(&env, NULL);


  aplatform->getApplication().initialize(aplatform);
  // platform->onResize(screenWidth, screenHeight);
  aplatform->getApplication().initializeContent();
  aplatform->renderLoop();
  aplatform->deinitializeRenderer();
  pthread_exit(0);

  return 0;
}

JNIEnv *
AndroidPlatform::getJNIEnv() const {
  if (gJavaVM == NULL) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JavaVM is null");
    return NULL;
  }

  JNIEnv * Myenv = NULL;
  gJavaVM->GetEnv((void**)&Myenv, JNI_VERSION_1_6);
  if (Myenv == NULL) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Env is null");
  }
  return Myenv;
}

std::shared_ptr<AndroidPlatform> platform;

extern "C" {

void Java_com_sometrik_framework_MyGLRenderer_onResize(JNIEnv* env, jobject thiz, float x, float y) {
  platform->setDisplayWidth(x);
  platform->setDisplayHeight(y);

  ResizeEvent ev(platform->getTime(), x / platform->getDisplayScale(), y / platform->getDisplayScale(), x, y);
  platform->queueEvent(platform->getActiveViewId(), ev);
}

void Java_com_sometrik_framework_FrameWork_menuPressed(JNIEnv* env, jobject thiz) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "menu pressed: env = %p", env);

  CommandEvent ce(platform->getTime(), FW_ID_MENU);
  platform->queueEvent(platform->getActiveViewId(), ce);
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int mode, int fingerIndex, long time, float x, float y) {
  x /= platform->getDisplayScale();
  y /= platform->getDisplayScale();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_DOWN, x, y, fingerIndex);
      platform->queueEvent(platform->getActiveViewId(), ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_MOVE, x, y, fingerIndex);
      platform->queueEvent(platform->getActiveViewId(), ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_UP, x, y, fingerIndex);
      platform->queueEvent(platform->getActiveViewId(), ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_MyGLRenderer_onInitElement(JNIEnv* env, jobject thiz, double timestamp, int viewId) {
  InitEvent ev(timestamp);
  platform->queueEvent(viewId, ev);
}

jboolean Java_com_sometrik_framework_MyGLRenderer_onUpdate(JNIEnv* env, jobject thiz, double timestamp, int viewId) {
  UpdateEvent ev(timestamp);
  platform->queueEvent(viewId, ev);
  return ev.isRedrawNeeded();
}

static JavaVM * gJavaVM = 0;
void Java_com_sometrik_framework_FrameWork_onInit(JNIEnv* env, jobject thiz, jobject assetManager, float screenWidth, float screenHeight, float displayScale, bool hasEs3) {
  if (!platform.get()) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");
    const char* glslVersion = "#version 100"; // "#version es 300"

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    platform = std::make_shared<AndroidPlatform>(env, assetManager, thiz, displayScale, glslVersion, hasEs3);
  }

  FWApplication * application = applicationMain();
  platform->setApplication(application);


  platform->setDisplayWidth(screenWidth);
  platform->setDisplayHeight(screenHeight);

#ifdef USE_NATIVE_SURFACE

  if (gJavaVM) {
  platform->setJavaVM(gJavaVM);
  }
  platform->startThread();
#else
  if (gJavaVM) {
    gJavaVM->AttachCurrentThread(&env, NULL);
    platform->setJavaVM(gJavaVM);
  }
  application->initialize(platform.get());
  // platform->onResize(screenWidth, screenHeight);
  application->initializeContent();
#endif
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
}

void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface, int surfaceId) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "going for it");
  ANativeWindow * window = 0;
  if (surface != 0) window = ANativeWindow_fromSurface(env, surface);
  AndroidConfigurationEvent ev(platform->getTime(), window);
  platform->queueEvent(surfaceId, ev);
}

void Java_com_sometrik_framework_MyGLRenderer_nativeOnDraw(JNIEnv* env, jobject thiz) {
  DrawEvent ev(platform->getTime());
  platform->queueEvent(platform->getActiveViewId(), ev);
}

void Java_com_sometrik_framework_FrameWork_buttonClicked(JNIEnv* env, jobject thiz, jint id) {
  TouchEvent ev(platform->getTime(), TouchEvent::ACTION_CLICK);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_nativeOnResume(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(platform->getTime(), SysEvent::RESUME);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnPause(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(platform->getTime(), SysEvent::PAUSE);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStop(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(platform->getTime(), SysEvent::STOP);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnRestart(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(platform->getTime(), SysEvent::RESTART);
  platform->queueEvent(appId, ev);
}
  
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, jint id, jstring jtext) {
  const char * text = env->GetStringUTFChars(jtext, 0);
  TextEvent ev(platform->getTime(), text);
  env->ReleaseStringUTFChars(jtext, text);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FWPicker_pickerOptionSelected(JNIEnv* env, jobject thiz, jint id, jint position){
  CommandEvent ev(platform->getTime(), id, position);
  platform->queueEvent(id, ev);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JNI_Onload on AndroidPlatform");
  gJavaVM = vm;
  if (gJavaVM == NULL){
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "problem on load. JavaVm is null");

  }

  return JNI_VERSION_1_6;
}
}
