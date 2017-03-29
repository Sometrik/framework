/*
 * Copyright (C) Sometrik oy 2015
 *
 */
//#undef GL_GLES_PROTOTYPES
#include <dlfcn.h>
#include <string.h>
#include <android/log.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include <android/bitmap.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>

#include <FWPlatform.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidSoundCanvas.h>
#include <AndroidLogger.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include <EventQueue.h>
#include <GL.h>


class JavaCache {

public:
  JavaCache(JNIEnv * env) {
    env->GetJavaVM(&javaVM);

    nativeCommandClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/NativeCommand"));
    frameworkClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/FrameWork"));
    systemClass = (jclass) env->NewGlobalRef(env->FindClass("java/lang/System"));
    jclass fileClass = env->FindClass("java/io/File");
    jclass contextWrapperClass = env->FindClass("android/content/ContextWrapper");

    loadPrefsValueMethod = env->GetMethodID(frameworkClass, "addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V");
    nativeCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BI)V");
    nativeListCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BIII)V");
    sendCommandMethod = env->GetStaticMethodID(frameworkClass, "sendMessage", "(Lcom/sometrik/framework/FrameWork;Lcom/sometrik/framework/NativeCommand;)V");
    getDatabasePathMethod = env->GetMethodID(contextWrapperClass, "getDatabasePath", "(Ljava/lang/String;)Ljava/io/File;");
    getPathMethod = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");

    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(contextWrapperClass);

    if (env->ExceptionCheck()){

    }
  }

  ~JavaCache() {
    JNIEnv * env = getJNIEnv();
      env->DeleteGlobalRef(nativeCommandClass);
      env->DeleteGlobalRef(frameworkClass);
  }

  JNIEnv * getJNIEnv() {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "cache getJNIENv called");
    if (javaVM == NULL){
      __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "VM is null");
    }

    JNIEnv *Myenv = NULL;
    javaVM->GetEnv((void**)&Myenv, JNI_VERSION_1_6);
    if (Myenv == NULL){
       __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Env is null");
     }
    return Myenv;
  }

  jclass nativeCommandClass;
  jclass frameworkClass;
  jclass systemClass;

  jmethodID loadPrefsValueMethod;
  // jmethodID addPrefsValueMethod;
  jmethodID storeValueMethod;
  // jmethodID currentTimeMillisMethod;
  jmethodID nativeCommandConstructor;
  jmethodID nativeListCommandConstructor;
  jmethodID sendCommandMethod;
  jmethodID getDatabasePathMethod;
  jmethodID getPathMethod;

private:
  JavaVM * javaVM;
};

class AndroidPlatform: public FWPlatform {
public:
 AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, JavaVM * _javaVM, MobileAccount * _account)
   : FWPlatform(_display_scale),
     account(_account),
    javaCache(JavaCache(_env)),
    gJavaVM(_javaVM) {
      
    framework = _env->NewGlobalRef(_framework);
    mgr = _env->NewGlobalRef(_mgr);
    canvasCache = std::make_shared<canvas::AndroidCache>(_env, _mgr);
    clientCache = std::make_shared<AndroidClientCache>(_env);
    sendCommand2(Command(Command::CREATE_PLATFORM, getParentInternalId(), getInternalId()));

#ifdef HAS_SOUNDCANVAS
    soundCache = std::make_shared<AndroidSoundCache>(_env, _mgr);
#endif
  }
  
  JavaVM * getJavaVM() { return gJavaVM; }

  std::string getBundleFilename(const char * filename) override;
  std::string getLocalFilename(const char * filename, FileType type) override;
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::AndroidContextFactory(mgr, canvasCache, getDisplayScale()));
  }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new AndroidClientFactory(clientCache));
  }
  void createFBO(int flags) { }

#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<AndroidSoundCanvas>(soundCache);
  }
#endif

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new AndroidLogger(name));
  }
  void sendCommand2(const Command & command) override;
  bool initializeRenderer(int opengl_es_version, ANativeWindow * _window);
  void deinitializeRenderer();
  void startThread();
  void joinThread();
  void renderLoop();
  static void* threadStartCallback(void *myself);

  JNIEnv* getJNIEnv() const;
//  JavaCache getJavaCache() const { return &javaCache; }

  void queueEvent(int internal_id, const Event & ev) {
    eventqueue.push(internal_id, ev);
  }

  void pushEvent(const Event & ev) {
    queueEvent(getInternalId(), ev);
  }

  void onOpenGLInitEvent(OpenGLInitEvent & _ev) override;
  void onSysEvent(SysEvent & ev) override;

 protected:
  jbyteArray convertToByteArray(const std::string & s);

private:
  pthread_t _threadId;
  JavaCache javaCache;

  EGLDisplay display = 0;
  EGLSurface surface = 0;
  EGLContext context = 0;
  std::shared_ptr<canvas::AndroidCache> canvasCache;
  std::shared_ptr<AndroidClientCache> clientCache;
  std::shared_ptr<AndroidSoundCache> soundCache;

  MobileAccount * account;
  ANativeWindow * window = 0;
  JavaVM * gJavaVM = 0;
  jobject mgr;
  jobject framework;
  jobject handler;
  EventQueue eventqueue;
  bool canDraw = false, isPaused = false, isDestroyed = false;
};

#include <FWApplication.h>

#include <pthread.h>

#include <TouchEvent.h>
#include <PurchaseEvent.h>
#include <TimerEvent.h>
#include <ValueEvent.h>
#include <SysEvent.h>
#include <CommandEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <AndroidOpenGLInitEvent.h>

#include <android_fopen.h>
#include <unistd.h>

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
  case DATABASE:{

    jstring jfilename = env->NewStringUTF(filename);
    jobject file = env->CallObjectMethod(framework, javaCache.getDatabasePathMethod, jfilename);
    jstring jdatabasePath = (jstring)env->CallObjectMethod(file, javaCache.getPathMethod);
    const char * cdatabasePath = env->GetStringUTFChars(jdatabasePath, JNI_FALSE);
    std::string databasePath = std::string(cdatabasePath);
    env->DeleteLocalRef(jfilename);
    env->ReleaseStringUTFChars(jdatabasePath, cdatabasePath);
    env->DeleteLocalRef(jdatabasePath);
    env->DeleteLocalRef(file);

    //put database here
    return databasePath;
  }
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
AndroidPlatform::sendCommand2(const Command & command) {
  if (!getActiveViewId() && (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_OPENGL_VIEW)) {
    setActiveViewId(command.getChildInternalId());
  }
  
  auto env = getJNIEnv();
  int commandTypeId = int(command.getType());
  auto textValue = command.getTextValue();
  auto textValue2 = command.getTextValue2();
  jbyteArray jtextValue = 0, jtextValue2 = 0;
  if (!textValue.empty()) jtextValue = convertToByteArray(textValue);
  if (!textValue2.empty()) jtextValue2 = convertToByteArray(textValue2);
  
  // SET_TEXT_VALUE check and new listcommand constructor should be refactored. ListView creation needs work
  jobject jcommand;
  if (command.getType() == Command::SET_TEXT_DATA) {
    jcommand = env->NewObject(javaCache.nativeCommandClass, javaCache.nativeListCommandConstructor, framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2, command.getFlags(), command.getRow(), command.getColumn());
  } else {
    jcommand = env->NewObject(javaCache.nativeCommandClass, javaCache.nativeCommandConstructor, framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2, command.getFlags());
  }
  env->CallStaticVoidMethod(javaCache.frameworkClass, javaCache.sendCommandMethod, framework, jcommand);
  
  env->DeleteLocalRef(jcommand);
  if (jtextValue) env->DeleteLocalRef(jtextValue);
  if (jtextValue2) env->DeleteLocalRef(jtextValue2);
  
  if (command.getType() == Command::SHOW_MESSAGE_DIALOG || command.getType() == Command::SHOW_INPUT_DIALOG || command.getType() == Command::SHOW_ACTION_SHEET) {
    modal_result_value = 0;
    modal_result_text = "";
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "starting modal run loop");
    renderLoop();
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "ending modal run loop");
  }
}

bool
AndroidPlatform::initializeRenderer(int opengl_es_version, ANativeWindow * _window) {
  canDraw = true;
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
  canDraw = false;

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

  bool exit_loop = false;
  while (!isDestroyed && !exit_loop) {
    std::vector<std::pair<int, std::shared_ptr<Event> > > evs;
    evs.push_back(eventqueue.pop());
    while (!eventqueue.empty()) {
      evs.push_back(eventqueue.pop());
    }

    bool redraw = false, update_sent = false;    
    std::pair<int, std::shared_ptr<UpdateEvent> > update_ev;
    for (auto & ev : evs) {
      if (dynamic_cast<UpdateEvent*>(ev.second.get())) {
	if (update_sent) {
	  // getLogger().println("skipping update event!");
	  continue;
	} else update_sent = true;
      }
      
      postEvent(ev.first, *ev.second.get());
      
      auto ev2 = dynamic_cast<SysEvent*>(ev.second.get());
      if (ev2) {
        if (ev2->getType() == SysEvent::END_MODAL) {
          getLogger().println("exiting loop after SysEvent::END_MODAL");
          exit_loop = true;
        } else if (ev2->getType() == SysEvent::DESTROY) {
          getLogger().println("exiting loop after SysEvent::DESTROY");
          exit_loop = true;
        }
      }

      if (ev.second.get()->isRedrawNeeded()) {
	redraw = true;
      }
    }

    if (canDraw && surface && redraw) {
      DrawEvent dev(getTime());
      postEvent(getActiveViewId(), dev);
      
      if (!eglSwapBuffers(display, surface)) {
	getLogger().println("error eglSwapBuffers");
      }
    }
  }

  getLogger().println("Looping Louie is out");
}

void AndroidPlatform::onOpenGLInitEvent(OpenGLInitEvent & _ev) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "platform OpenGLInitEvent");
  if (_ev.getOpenGLVersion() < 3) {
    int results = importGLInit();
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "importResult = %u", results);
  } else {
    int results = importGLInit();
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "importResult = %u", results);
  }
  auto & ev = dynamic_cast<AndroidOpenGLInitEvent&>(_ev);
  if (ev.getWindow()) {
    initializeRenderer(ev.getOpenGLVersion(), ev.getWindow());
    ev.requestRedraw();
    FWPlatform::onOpenGLInitEvent(_ev);
  }
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
  if (aplatform->isInitialized()){
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "aplatform is initialized");
  } else {
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "aplatform isn't initialized");
  }

  JNIEnv * env;
  aplatform->getJavaVM()->AttachCurrentThread(&env, NULL);
  FWApplication * application = applicationMain();
  aplatform->addChild(std::shared_ptr<Element>(application));
  aplatform->renderLoop();
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "Application is exiting");
  aplatform->deinitializeRenderer();
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

void Java_com_sometrik_framework_FrameWork_keyPressed(JNIEnv* env, jobject thiz, double timestamp, int keyId, int viewId) {
  if (keyId == 4) {
    int poppedView = platform->popViewBackHistory();
    if (poppedView != 0) {
      Command co(Command::SET_INT_VALUE, poppedView);
      co.setValue(2);
      platform->sendCommand2(co);
    } else {
      Command co(Command::QUIT_APP, poppedView);
      platform->sendCommand2(co);
      //TODO
      // Wrong Thread
    }
  } else if (keyId == 82) {
    CommandEvent ce(timestamp, FW_ID_MENU);
    platform->queueEvent(platform->getActiveViewId(), ce);
  } else {
    CommandEvent ce(timestamp, keyId);
    platform->queueEvent(viewId, ce);
  }
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int viewId, int mode, int fingerIndex, double timestamp, float x, float y) {
  x /= platform->getDisplayScale();
  y /= platform->getDisplayScale();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DOWN, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DRAG, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_UP, x, y, fingerIndex);
      platform->queueEvent(viewId, ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_FrameWork_flushTouchEvent(JNIEnv* env, jobject thiz, double timestamp, int viewId, int mode) {
  switch (mode) {
  case 1:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DOWN, true);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DRAG, true);
      platform->queueEvent(viewId, ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_UP, true);
      platform->queueEvent(viewId, ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_FrameWork_onUpdate(JNIEnv* env, jclass clazz, double timestamp, int viewId) {
  UpdateEvent ev(timestamp);
  platform->queueEvent(viewId, ev);
}

static JavaVM * gJavaVM = 0;
void Java_com_sometrik_framework_FrameWork_onInit(JNIEnv* env, jobject thiz, jobject assetManager, float screenWidth, float screenHeight, float displayScale, jstring jemail, jstring jlanguage, jstring jcountry) {
  if (!platform) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");


    const char * email = env->GetStringUTFChars(jemail, NULL);
    const char * language = env->GetStringUTFChars(jlanguage, NULL);
    const char * country = env->GetStringUTFChars(jcountry, NULL);

    MobileAccount account = MobileAccount(email, language, country);

    env->ReleaseStringUTFChars(jlanguage, language);
    env->ReleaseStringUTFChars(jemail, email);
    env->ReleaseStringUTFChars(jcountry, country);

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    platform = new AndroidPlatform(env, assetManager, thiz, displayScale, gJavaVM, &account);
    platform->setDisplayWidth(screenWidth);
    platform->setDisplayHeight(screenHeight);

    platform->startThread();

    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
  }
}

  void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface, int surfaceId, int gl_version) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "setting native surface on androidPlatform");
  if (surface != 0) {
    ANativeWindow * window = 0;
    window = ANativeWindow_fromSurface(env, surface);
    AndroidOpenGLInitEvent ev(platform->getTime(), gl_version, true, window);
    platform->queueEvent(platform->getInternalId(), ev);
  }
}


 void Java_com_sometrik_framework_FrameWork_nativeSurfaceDestroyed(JNIEnv* env, jobject thiz, int surfaceId, int gl_version) {
   __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "native surface destroyed on androidPlatform");
   platform->deinitializeRenderer();  
 }

 void Java_com_sometrik_framework_FrameWork_endModal(JNIEnv* env, jobject thiz, double timestamp, int value, jbyteArray jarray) {
   string text;
   if (jarray) {
     jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
     text = string((const char *)content_array, env->GetArrayLength(jarray));
     env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
   }
   __android_log_print(ANDROID_LOG_INFO, "Sometrik", "native endModal: %d %s", value, text.c_str());
   SysEvent ev(timestamp, SysEvent::END_MODAL);
   ev.setValue(value);
   ev.setTextValue(text);
   platform->queueEvent(platform->getInternalId(), ev);
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
void Java_com_sometrik_framework_FrameWork_languageChanged(JNIEnv* env, jobject thiz, double timestamp, int appId, jstring language) {
  const char * clanguage = env->GetStringUTFChars(language, NULL);
  SysEvent ev(timestamp, SysEvent::LANGUAGE_CHANGED);
  ev.setTextValue(clanguage);
  platform->queueEvent(appId, ev);
  env->ReleaseStringUTFChars(language, clanguage);
}
void Java_com_sometrik_framework_FrameWork_memoryWarning(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(timestamp, SysEvent::MEMORY_WARNING);
  platform->queueEvent(appId, ev);
}
  
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, jbyteArray jarray) {
  string text;
  if (jarray) {
    jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
    text = string((const char *) content_array, env->GetArrayLength(jarray));
    env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
  }
  ValueEvent ev(timestamp, text);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_intChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, jint changedInt){
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "textChangedEvent: %u", changedInt);
  ValueEvent ev(timestamp, changedInt);
  platform->queueEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_timerEvent(JNIEnv* env, jobject thiz, double timestamp, jint viewId, jint timerId){
  TimerEvent ev(timestamp, timerId);
  platform->queueEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_setNativeActiveView(JNIEnv* env, jobject thiz, double timestamp, jint activeView, bool recordHistory) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "setActivewView: %u", activeView);
  if (platform->getActiveViewId() != 0 && recordHistory) {
    platform->addToHistory(platform->getActiveViewId());
  }
  platform->setActiveViewId(activeView);
  //TODO
  // Wrong thread. Send event instead
}

void Java_com_sometrik_framework_FrameWork_OnPurchaseEvent(JNIEnv* env, jclass clazz, double timestamp, jint applicationId, jstring productId, bool newPurchase, double purchaseTime){
  const char * cstring = env->GetStringUTFChars(productId, 0);
  //PurchaseEvent Type not used yet. PurchaseTime is not sent //TODO
  PurchaseEvent ev(timestamp, cstring, PurchaseEvent::PURCHASE_STATUS, newPurchase);
 //Id not used. Sent to 0 //TODO
  platform->queueEvent(applicationId, ev);
  env->ReleaseStringUTFChars(productId, cstring);
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
