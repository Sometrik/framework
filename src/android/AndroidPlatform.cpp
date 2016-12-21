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
#include <ValueEvent.h>
#include <SysEvent.h>
#include <CommandEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <AndroidOpenGLInitEvent.h>

#include <android_fopen.h>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

extern FWApplication * applicationMain();

std::string AndroidPlatform::getBundleFilename(const char * filename) {
  return filename;
}

std::string AndroidPlatform::getLocalFilename(const char * filename, FileType type) {

  auto env = getJNIEnv();
//  jstring path;
//  jstring jfilename;
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

jbyteArray
AndroidPlatform::convertToByteArray(const std::string & s) {
  auto env = getJNIEnv();
  const jbyte * pNativeMessage = reinterpret_cast<const jbyte*>(s.c_str());
  jbyteArray bytes = env->NewByteArray(s.size());
  env->SetByteArrayRegion(bytes, 0, s.size(), pNativeMessage);
  return bytes;
}

void
AndroidPlatform::sendCommand(const Command & command) {
  getLogger().println("sending command");
  FWPlatform::sendCommand(command);

  auto env = getJNIEnv();
  int commandTypeId = int(command.getType());
  auto textValue = command.getTextValue();
  auto textValue2 = command.getTextValue2();
  jbyteArray jtextValue = 0, jtextValue2 = 0;
  if (!textValue.empty()) jtextValue = convertToByteArray(textValue);
  if (!textValue2.empty()) jtextValue2 = convertToByteArray(textValue2);
  
  jobject jcommand = env->NewObject(javaCache.nativeCommandClass, javaCache.nativeCommandConstructor, framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2, command.getFlags());
  env->CallStaticVoidMethod(javaCache.frameworkClass, javaCache.sendCommandMethod, framework, jcommand);
  
  env->DeleteLocalRef(jcommand);
  if (jtextValue) env->DeleteLocalRef(jtextValue);
  if (jtextValue2) env->DeleteLocalRef(jtextValue2);
  
  if (command.getType() == Command::SHOW_MESSAGE_DIALOG || command.getType() == Command::SHOW_INPUT_DIALOG || command.getType() == Command::SHOW_ACTION_SHEET) {
    modal_result_value = 0;
    modal_result_text = "";
    renderLoop();
  }
}

double
AndroidPlatform::getTime() const {
#if 1
  struct timeval tv;
  struct timezone tz;
  int r = gettimeofday(&tv, &tz);
  double t = 0;
  if (r == 0) {
    t = (double)tv.tv_sec + tv.tv_usec / 1000000.0;
  }
  return t;
#else
  auto env = getJNIEnv();
  double currentTime = (double)env->CallStaticLongMethod(javaCache.systemClass, javaCache.currentTimeMillisMethod);

  return currentTime;
#endif
}

bool
AndroidPlatform::initializeRenderer(int opengl_es_version, ANativeWindow * _window) {
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
    int version = opengl_es_version >> 16;

    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "version check: %d", version);
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, version, EGL_NONE };

    if ((_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglGetDisplay() returned error %d", eglGetError());
      return false;
    }
    if (!eglInitialize(_display, 0, 0)) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglInitialize() returned error %d", eglGetError());
      return false;
    }

    if (!eglChooseConfig(_display, attribs, &_config, 1, &numConfigs)) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglChooseConfig() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format)) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglGetConfigAttrib() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

    if (!(_surface = eglCreateWindowSurface(_display, _config, window, 0))) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglCreateWindowSurface() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!(_context = eglCreateContext(_display, _config, 0, contextAttribs))) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglCreateContext() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglMakeCurrent(_display, _surface, _surface, _context)) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglMakeCurrent() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    if (!eglQuerySurface(_display, _surface, EGL_WIDTH, &width) || !eglQuerySurface(_display, _surface, EGL_HEIGHT, &height)) {
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "eglQuerySurface() returned error %d", eglGetError());
      //         destroy();
      return false;
    }

    display = _display;
    surface = _surface;
    context = _context;

    getLogger().println("renderer initialized");
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
    window = 0;
  }
}

void
AndroidPlatform::startThread() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 1");
  pthread_create(&_threadId, 0, threadStartCallback, this);
}

void
AndroidPlatform::joinThread() {
  pthread_join(_threadId, 0);
}

void
AndroidPlatform::renderLoop() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Looping louie");

  while (!isDestroyed) {
    auto ev = eventqueue.pop();

    std::ostringstream s;
    s << "trying to dispatch event " << typeid(*ev.second.get()).name() << " to: " << ev.first ;
    getLogger().println(s.str());
    
    postEvent(ev.first, *ev.second.get());
    
    auto ev2 = dynamic_cast<SysEvent*>(ev.second.get());
    if (ev2 && (ev2->getType() == SysEvent::END_MODAL || ev2->getType() == SysEvent::DESTROY)) {
      break;
    }

    if (canDraw && surface && ev.second.get()->isRedrawNeeded()) {
      __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "I Wanna draw");
      DrawEvent dev(getTime());
      postEvent(getActiveViewId(), dev);
      
      if (!eglSwapBuffers(display, surface)) {
	getLogger().println("error eglSwapBuffers");
      }
    }
  }

  getLogger().println("Looping Louie is out");
}

void
AndroidPlatform::onOpenGLInitEvent(OpenGLInitEvent & _ev) {
  auto & ev = dynamic_cast<AndroidOpenGLInitEvent&>(_ev);
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "initializeRenderer reached");
  if (ev.getWindow()) {
    initializeRenderer(ev.getOpenGLVersion(), ev.getWindow());
    ev.requestRedraw();
    canDraw = true;
  } else {
    deinitializeRenderer();
    canDraw = false;
  }
  FWPlatform::onOpenGLInitEvent(_ev);
}

void
AndroidPlatform::onSysEvent(SysEvent & ev) {
  FWPlatform::onSysEvent(ev);

  switch (ev.getType()) {
  case SysEvent::PAUSE:
    isPaused = true;
    break;
  case SysEvent::RESUME:
    isPaused = false;
    break;
  case SysEvent::END_MODAL:
    modal_result_value = ev.getValue();
    modal_result_text = ev.getTextValue();
    break;
  case SysEvent::DESTROY:
    isDestroyed = true;
    break;
  case SysEvent::MEMORY_WARNING:
    //TODO
    break;
  case SysEvent::START:
    //TODO
    break;
  case SysEvent::STOP:
    //TODO
    break;
  case SysEvent::THREAD_TERMINATED:
    //TODO
    break;
  }
}

void* AndroidPlatform::threadStartCallback(void *myself) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 2");

  AndroidPlatform * aplatform = (AndroidPlatform*) myself;

  JNIEnv * env;
  aplatform->getJavaVM()->AttachCurrentThread(&env, NULL);

  FWApplication * application = applicationMain();
  aplatform->addChild(std::shared_ptr<Element>(application));

  aplatform->renderLoop();
  aplatform->deinitializeRenderer();
//  Command c = Command(Command::Type::QUIT_APP, 0, 0);
//  aplatform->sendCommand(c);
  aplatform->getJavaVM()->DetachCurrentThread();
  
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

static AndroidPlatform * platform = 0;

extern "C" {

void Java_com_sometrik_framework_FrameWork_onResize(JNIEnv* env, jclass clazz, double timestamp, float x, float y, int viewId) {
  platform->setDisplayWidth(x);
  platform->setDisplayHeight(y);

  ResizeEvent ev(timestamp, x / platform->getDisplayScale(), y / platform->getDisplayScale(), x, y);
  platform->queueEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_menuPressed(JNIEnv* env, jobject thiz, double timestamp, int viewId) {
  CommandEvent ce(timestamp, FW_ID_MENU);
  platform->queueEvent(platform->getActiveViewId(), ce);
}

void Java_com_sometrik_framework_FrameWork_keyPressed(JNIEnv* env, jobject thiz, double timestamp, int keyId, int viewId) {
  CommandEvent ce(timestamp, keyId);
  platform->queueEvent(viewId, ce);
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int viewId, int mode, int fingerIndex, long time, float x, float y) {
  x /= platform->getDisplayScale();
  y /= platform->getDisplayScale();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_DOWN, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_MOVE, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(time / 1000.0, TouchEvent::ACTION_UP, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_MyGLRenderer_onInitElement(JNIEnv* env, jobject thiz, double timestamp, int viewId) {
}

void Java_com_sometrik_framework_FrameWork_onUpdate(JNIEnv* env, jclass clazz, double timestamp, int viewId) {
  UpdateEvent ev(timestamp);
  platform->queueEvent(viewId, ev);
}

static JavaVM * gJavaVM = 0;
void Java_com_sometrik_framework_FrameWork_onInit(JNIEnv* env, jobject thiz, jobject assetManager, float screenWidth, float screenHeight, float displayScale) {
  bool start_thread = false;
  if (!platform) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    platform = new AndroidPlatform(env, assetManager, thiz, displayScale);
    platform->setDisplayWidth(screenWidth);
    platform->setDisplayHeight(screenHeight);

  
    start_thread = true;
  }

  if (gJavaVM) {
    platform->setJavaVM(gJavaVM);
  }

  if (start_thread) {
    platform->startThread();
  }
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
}

  void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface, int surfaceId, int gl_version) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "setting native surface on androidPlatform");
  ANativeWindow * window = 0;
  if (surface != 0) window = ANativeWindow_fromSurface(env, surface);
  AndroidOpenGLInitEvent ev(platform->getTime(), gl_version, true, window);
  platform->queueEvent(platform->getInternalId(), ev);
}


 void Java_com_sometrik_framework_FrameWork_nativeSurfaceDestroyed(JNIEnv* env, jobject thiz, int surfaceId, int gl_version) {
   __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "native surface destroyed on androidPlatform");
  AndroidOpenGLInitEvent ev(platform->getTime(), gl_version, true, 0);
  platform->queueEvent(platform->getInternalId(), ev);
 }

 void Java_com_sometrik_framework_FrameWork_endModal(JNIEnv* env, jobject thiz, double timestamp, int value, jbyteArray jarray) {
   __android_log_print(ANDROID_LOG_INFO, "Sometrik", "endModal");
   string text;
   if (jarray) {
     jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
     text = string((const char *)content_array, env->GetArrayLength(jarray));
     env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
   }
   __android_log_print(ANDROID_LOG_INFO, "Sometrik", "endModal: %d %s", value, text.c_str());
   SysEvent ev(timestamp, SysEvent::END_MODAL);
   ev.setValue(value);
   ev.setTextValue(text);
   platform->queueEvent(platform->getInternalId(), ev);
 }

void Java_com_sometrik_framework_FrameWork_buttonClicked(JNIEnv* env, jobject thiz, double timestamp, jint id) {
  TouchEvent ev(timestamp, TouchEvent::ACTION_CLICK);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_nativeOnResume(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::RESUME);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnPause(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::PAUSE);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStop(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::STOP);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStart(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::START);
  platform->queueEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnDestroy(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::DESTROY);
  platform->queueEvent(appId, ev);
  platform->joinThread();
  delete platform;
}
  
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, jstring jtext) {
  const char * text = env->GetStringUTFChars(jtext, 0);
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "textChangedEvent: %s", text);
  ValueEvent ev(timestamp, text);
  env->ReleaseStringUTFChars(jtext, text);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FWPicker_pickerOptionSelected(JNIEnv* env, jobject thiz, double timestamp, jint id, jint position){
  ValueEvent ev(timestamp, position);
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
