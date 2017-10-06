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
#include <android/bitmap.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>

#include <FWPlatform.h>
#include <AndroidClient.h>
#include <AndroidSoundCanvas.h>
#include <AndroidLogger.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include <GL.h>

#include <PosixThread.h>

#include <FWApplication.h>
#include <FWDefs.h>

#include <TouchEvent.h>
#include <PurchaseEvent.h>
#include <TimerEvent.h>
#include <ValueEvent.h>
#include <VisibilityEvent.h>
#include <SysEvent.h>
#include <CommandEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>
#include <AndroidOpenGLInitEvent.h>

#include <android_fopen.h>
#include <unistd.h>

#include <string>

using namespace std;

static jbyteArray convertToByteArray(JNIEnv * env, const std::string & s) {
  const jbyte * pNativeMessage = reinterpret_cast<const jbyte*>(s.c_str());
  jbyteArray bytes = env->NewByteArray(s.size());
  env->SetByteArrayRegion(bytes, 0, s.size(), pNativeMessage);
  return bytes;
}

class JavaCache {

public:
  JavaCache(JNIEnv * env, jobject & _framework) {
    env->GetJavaVM(&javaVM);

    framework = env->NewGlobalRef(_framework);

    nativeCommandClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/NativeCommand"));
    frameworkClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/FrameWork"));
    systemClass = (jclass) env->NewGlobalRef(env->FindClass("java/lang/System"));
    jclass fileClass = env->FindClass("java/io/File");
    jclass contextWrapperClass = env->FindClass("android/content/ContextWrapper");

    loadPrefsValueMethod = env->GetMethodID(frameworkClass, "addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V");
    nativeCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BI)V");
    nativeListCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BIIIIII)V");
    sendCommandMethod = env->GetStaticMethodID(frameworkClass, "sendMessage", "(Lcom/sometrik/framework/FrameWork;Lcom/sometrik/framework/NativeCommand;)V");
    getPathMethod = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");

    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(contextWrapperClass);

    if (env->ExceptionCheck()){

    }
  }

  ~JavaCache() {
#if 0
    env->DeleteGlobalRef(framework);
    env->DeleteGlobalRef(nativeCommandClass);
    env->DeleteGlobalRef(frameworkClass);
    env->DeleteGlobalRef(systemClass);
#endif
  }

  JavaVM & getJavaVM() { return *javaVM; }

  JavaVM * javaVM = 0;

  jobject framework;

  jclass nativeCommandClass;
  jclass frameworkClass;
  jclass systemClass;

  jmethodID loadPrefsValueMethod;
  jmethodID nativeCommandConstructor;
  jmethodID nativeListCommandConstructor;
  jmethodID sendCommandMethod;
  jmethodID getPathMethod;
};

class AndroidThread;
class AndroidNativeThread;

class AndroidPlatform : public FWPlatform {
public:
  friend class AndroidNativeThread;
  
  AndroidPlatform(JNIEnv * _env, jobject & _mgr, jobject & _framework) : javaCache(_env, _framework) {
    registerElement(this);

    canvasCache = std::make_shared<canvas::AndroidCache>(_env, _mgr);
    clientCache = std::make_shared<AndroidClientCache>(_env);

#ifdef HAS_SOUNDCANVAS
    soundCache = std::make_shared<AndroidSoundCache>(_env, _mgr);
#endif
  }

  void createFBO(int flags) { }

#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<AndroidSoundCanvas>(soundCache);
  }
#endif

  bool initializeRenderer(int opengl_es_version, ANativeWindow * _window);
  void deinitializeRenderer();
  void renderLoop();

  void startModal() override {
    renderLoop();
  }

  void endModal() override {
    exit_loop = true;
  }

  void onOpenGLInitEvent(OpenGLInitEvent & _ev) override {
    if (_ev.getOpenGLVersion() < 190000) {
      importGLInit();
    } else {
      importGLInit();
    }
    auto & ev = dynamic_cast<AndroidOpenGLInitEvent&>(_ev);
    if (ev.getWindow()) {
      initializeRenderer(ev.getOpenGLVersion(), ev.getWindow());
      ev.requestRedraw();
      FWPlatform::onOpenGLInitEvent(_ev);
    }
  }

  void onSysEvent(SysEvent & ev) override {
    FWPlatform::onSysEvent(ev);

    switch (ev.getType()) {
    case SysEvent::PAUSE:
      isPaused = true;
      break;
    case SysEvent::RESUME:
      isPaused = false;
      break;
    case SysEvent::END_MODAL:
      getThread().setModalResultValue(ev.getValue());
      getThread().setModalResultText(ev.getTextValue());
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

  const std::shared_ptr<AndroidClientCache> & getClientCache() const { return clientCache; }
  const std::shared_ptr<canvas::AndroidCache> & getCanvasCache() const { return canvasCache; }
  JavaCache & getJavaCache() { return javaCache; }
  
private:
  JavaCache javaCache;

  EGLDisplay display = 0;
  EGLSurface surface = 0;
  EGLContext context = 0;
  std::shared_ptr<canvas::AndroidCache> canvasCache;
  std::shared_ptr<AndroidClientCache> clientCache;
  std::shared_ptr<AndroidSoundCache> soundCache;

  ANativeWindow * window = 0;
  bool canDraw = false, isPaused = false, isDestroyed = false;
  bool exit_loop = false;
};

extern FWApplication * applicationMain();

class AndroidThread : public PosixThread {
public:
  AndroidThread(PlatformThread * _parent_thread, AndroidPlatform * _platform, AAssetManager * _asset_manager, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_parent_thread, _platform, _runnable), asset_manager(_asset_manager), javaCache(&(_platform->getJavaCache())) {
  }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    const AndroidPlatform & androidPlatform = dynamic_cast<const AndroidPlatform&>(getPlatform());
    return std::unique_ptr<HTTPClientFactory>(new AndroidClientFactory(androidPlatform.getClientCache()));
  }
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    const AndroidPlatform & androidPlatform = dynamic_cast<const AndroidPlatform&>(getPlatform());
    return std::unique_ptr<canvas::ContextFactory>(new canvas::AndroidContextFactory(getAssetManager(), androidPlatform.getCanvasCache(), getDisplayScale()));
  }

  std::string getBundleFilename(const char * filename) override {
    return filename;
  }
  std::string getLocalFilename(const char * filename, FileType type) override {
    switch (type) {
    case DATABASE: return "/data/data/com.sometrik.kraphio/databases/" + std::string(filename);
    case CACHE_DATABASE: return "";
    case NORMAL: return "";
    }
    return "";
  }
  std::string loadTextAsset(const char * filename) override {
    string text;
    FILE * in = android_fopen(filename, "r");
    while (!feof(in)) {
      char b[256];
      size_t n = fread(b, 1, 256, in);
      text += string(b, n);
    }
    fclose(in);
    return text;
  }

  std::unique_ptr<Logger> createLogger(const std::string & name) const override {
    return std::unique_ptr<Logger>(new AndroidLogger(name));
  }

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) {
    AndroidPlatform & androidPlatform = dynamic_cast<AndroidPlatform&>(getPlatform());
    return std::make_shared<AndroidThread>(this, &androidPlatform, asset_manager, runnable);
  }

  int sendCommand(const Command & command) override {
    if (command.getType() == Command::CREATE_FORMVIEW || command.getType() == Command::CREATE_OPENGL_VIEW) {
      auto & app = getPlatform().getApplication();
      if (!app.getActiveViewId()) {
        app.setActiveViewId(command.getChildInternalId());
      }
    }

    int commandTypeId = int(command.getType());
    auto textValue = command.getTextValue();
    auto textValue2 = command.getTextValue2();
    jbyteArray jtextValue = 0, jtextValue2 = 0;
    if (!textValue.empty()) jtextValue = convertToByteArray(myEnv, textValue);
    if (!textValue2.empty()) jtextValue2 = convertToByteArray(myEnv, textValue2);

    // SET_TEXT_VALUE check and new listcommand constructor should be refactored. ListView creation needs work
    jobject jcommand = myEnv->NewObject(javaCache->nativeCommandClass, javaCache->nativeListCommandConstructor, javaCache->framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2, command.getFlags(), command.getRow(), command.getColumn(), command.getSheet(), command.getWidth(), command.getHeight());
    myEnv->CallStaticVoidMethod(javaCache->frameworkClass, javaCache->sendCommandMethod, javaCache->framework, jcommand);

    myEnv->DeleteLocalRef(jcommand);
    if (jtextValue) myEnv->DeleteLocalRef(jtextValue);
    if (jtextValue2) myEnv->DeleteLocalRef(jtextValue2);

    if (command.getType() == Command::SHOW_DIALOG ||
        command.getType() == Command::SHOW_MESSAGE_DIALOG ||
        command.getType() == Command::SHOW_INPUT_DIALOG ||
        command.getType() == Command::SHOW_ACTION_SHEET) {
      setModalResultValue(0);
      setModalResultText("");
      getPlatform().startModal();
      return getModalResultValue();
    } else if (command.getType() == Command::END_MODAL) {
      getPlatform().endModal();
    }
    return 0;
  }

  AAssetManager * getAssetManager() const { return asset_manager; }

protected:
  void initialize() override {
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup

    javaCache->getJavaVM().AttachCurrentThread(&myEnv, &args);
  }
  void deinitialize() override {
    javaCache->getJavaVM().DetachCurrentThread();
  }

  JavaCache * javaCache;
  JNIEnv * myEnv = 0;
  AAssetManager * asset_manager;
};

class AndroidMainThread : public AndroidThread {
public:
  AndroidMainThread(AndroidPlatform * _platform, AAssetManager * _asset_manager, std::shared_ptr<Runnable> & _runnable, const MobileAccount & _mobileAccount, const FWPreferences & _preferences)
  : AndroidThread(nullptr, _platform, _asset_manager, _runnable), mobileAccount(_mobileAccount), preferences(_preferences) { }

  void startRunnable() override {
    auto & androidPlatform = dynamic_cast<AndroidPlatform&>(getPlatform());

    androidPlatform.create();
    androidPlatform.initialize(this);
    androidPlatform.initializeChildren();
    androidPlatform.load();

    shared_ptr<FWApplication> application(applicationMain());
    application->setPreferences(preferences);
    application->setMobileAccount(mobileAccount);

    androidPlatform.addChild(application);
    androidPlatform.renderLoop();
    androidPlatform.deinitializeRenderer();
  }

private:
  MobileAccount mobileAccount;
  FWPreferences preferences;
};

static std::shared_ptr<AndroidMainThread> mainThread;
static FWPreferences initialPrefs;

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
AndroidPlatform::renderLoop() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Looping louie");
  
  exit_loop = false;
  
  while (!isDestroyed && !exit_loop) {
    auto evs = getThread().pollEvents();

    bool redraw = false, update_sent = false;    
    for (auto & ev : evs) {
      if (dynamic_cast<UpdateEvent*>(ev.second.get())) {
	if (update_sent) continue;
	update_sent = true;
      }
      
      postEvent(ev.first, *ev.second.get());
      
      auto ev2 = dynamic_cast<SysEvent*>(ev.second.get());
      if (ev2) {
        if (ev2->getType() == SysEvent::END_MODAL) {
          exit_loop = true;
        } else if (ev2->getType() == SysEvent::DESTROY) {
          exit_loop = true;
        } else if (ev2->getType() == SysEvent::PAUSE) {
        }
      }

      if (ev.second.get()->isRedrawNeeded()) {
	redraw = true;
      }
    }

    if (canDraw && surface && redraw) {
      DrawEvent dev;
      postEvent(getApplication().getActiveViewId(), dev);
      
      if (!eglSwapBuffers(display, surface)) {
        // Swap buffers failed
      }
    }
  }

  exit_loop = false;
}

#ifdef PROFILING
extern "C" void monstartup( char const * );
extern "C" void moncleanup();
#endif
extern "C" {

void Java_com_sometrik_framework_FrameWork_onResize(JNIEnv* env, jclass clazz, double timestamp, float x, float y, int viewId) {
  mainThread->setActualDisplayWidth(x);
  mainThread->setActualDisplayHeight(y);

  ResizeEvent ev(x / mainThread->getDisplayScale(), y / mainThread->getDisplayScale(), x, y);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_keyPressed(JNIEnv* env, jobject thiz, double timestamp, int keyId, int viewId) {
  auto & platform = mainThread->getPlatform();
  if (keyId == 4) {
    SysEvent ev(SysEvent::BACK);
    mainThread->sendEvent(platform.getApplication().getInternalId(), ev);
  } else if (keyId == 82) {
    CommandEvent ce(FW_ID_MENU);
    mainThread->sendEvent(platform.getApplication().getActiveViewId(), ce);
  } else {
    CommandEvent ce(keyId);
    mainThread->sendEvent(viewId, ce);
  }
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int viewId, int mode, int fingerIndex, double timestamp, float x, float y) {
  x /= mainThread->getDisplayScale();
  y /= mainThread->getDisplayScale();
  auto & platform = mainThread->getPlatform();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DOWN, x, y, fingerIndex);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DRAG, x, y, fingerIndex);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_UP, x, y, fingerIndex);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_FrameWork_flushTouchEvent(JNIEnv* env, jobject thiz, double timestamp, int viewId, int mode) {
  auto & platform = mainThread->getPlatform();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DOWN, true);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_DRAG, true);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(timestamp, TouchEvent::ACTION_UP, true);
      mainThread->sendEvent(viewId, ev);
    }
    break;
  }
}

void Java_com_sometrik_framework_FrameWork_onUpdate(JNIEnv* env, jclass clazz, double timestamp, int viewId) {
  UpdateEvent ev(timestamp);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_onInit(JNIEnv* env, jobject thiz, jobject assetManager, int screenWidth, int screenHeight, float displayScale, jstring jemail, jstring jlanguage, jstring jcountry) {
  if (!mainThread.get()) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");
    __android_log_print(ANDROID_LOG_INFO, "onInit", "oninit screenWidth: %d", screenWidth);
    __android_log_print(ANDROID_LOG_INFO, "onInit", "oninit screenHeight: %d", screenHeight);

    const char * email = env->GetStringUTFChars(jemail, NULL);
    const char * language = env->GetStringUTFChars(jlanguage, NULL);
    const char * country = env->GetStringUTFChars(jcountry, NULL);

    MobileAccount account = MobileAccount(email, language, country);

    env->ReleaseStringUTFChars(jlanguage, language);
    env->ReleaseStringUTFChars(jemail, email);
    env->ReleaseStringUTFChars(jcountry, country);

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    shared_ptr<Runnable> runnable;

    auto platform = new AndroidPlatform(env, assetManager, thiz);
    mainThread = make_shared<AndroidMainThread>(platform, manager, runnable, account, initialPrefs);
    mainThread->setActualDisplayWidth(screenWidth);
    mainThread->setActualDisplayHeight(screenHeight);
    mainThread->setDisplayScale(displayScale);
    mainThread->start();

//    setenv("CPUPROFILE", "/data/data/com.sometrik.kraphio/files/gmon.out", 1);
#ifdef PROFILING
    monstartup("framework.so");
#endif

    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
  }
}

void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface, int surfaceId, int gl_version, int width, int height) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "setting native surface on androidPlatform");
  mainThread->setActualDisplayWidth(width);
  mainThread->setActualDisplayHeight(height);
  if (surface != 0) {
    ANativeWindow * window = 0;
    window = ANativeWindow_fromSurface(env, surface);
    AndroidOpenGLInitEvent ev(gl_version, true, window);
    auto & platform = mainThread->getPlatform();
    mainThread->sendEvent(platform.getInternalId(), ev);
  }
}

 void Java_com_sometrik_framework_FrameWork_nativeSurfaceDestroyed(JNIEnv* env, jobject thiz, int surfaceId, int gl_version) {
   __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "native surface destroyed on androidPlatform");
   auto & androidPlatform = dynamic_cast<AndroidPlatform&>(mainThread->getPlatform());
   androidPlatform.deinitializeRenderer();
 }

 void Java_com_sometrik_framework_FrameWork_endModal(JNIEnv* env, jobject thiz, double timestamp, int value, jbyteArray jarray) {
   string text;
   if (jarray) {
     jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
     text = string((const char *)content_array, env->GetArrayLength(jarray));
     env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
   }
   __android_log_print(ANDROID_LOG_INFO, "Sometrik", "native endModal: %d %s", value, text.c_str());
   SysEvent ev(SysEvent::END_MODAL);
   ev.setValue(value);
   ev.setTextValue(text);
   auto & platform = mainThread->getPlatform();
   mainThread->sendEvent(platform.getInternalId(), ev);
 }

void Java_com_sometrik_framework_FrameWork_nativeOnResume(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(SysEvent::RESUME);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnPause(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(SysEvent::PAUSE);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStop(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(SysEvent::STOP);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStart(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  if (mainThread.get()) {
    SysEvent ev(SysEvent::START);
    mainThread->sendEvent(appId, ev);
  } else {
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "NO THREAD FOR SysEvent::START");
  }
}
void Java_com_sometrik_framework_FrameWork_nativeOnDestroy(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  mainThread->terminateThreads();
  SysEvent ev(SysEvent::DESTROY);
  auto & platform = mainThread->getPlatform();
  mainThread->sendEvent(appId, ev);
  // TODO: Wait for threads to terminate
#ifdef PROFILING
  moncleanup();
#endif

  mainThread = shared_ptr<AndroidMainThread>(0);
  delete &platform;
}
void Java_com_sometrik_framework_FrameWork_languageChanged(JNIEnv* env, jobject thiz, double timestamp, int appId, jstring language) {
  const char * clanguage = env->GetStringUTFChars(language, NULL);
  SysEvent ev(SysEvent::LANGUAGE_CHANGED);
  ev.setTextValue(clanguage);
  mainThread->sendEvent(appId, ev);
  env->ReleaseStringUTFChars(language, clanguage);
}
void Java_com_sometrik_framework_FrameWork_memoryWarning(JNIEnv* env, jobject thiz, double timestamp, int appId) {
  SysEvent ev(SysEvent::MEMORY_WARNING);
  mainThread->sendEvent(appId, ev);
}
  
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, jbyteArray jarray) {
  string text;
  if (jarray) {
    jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
    text = string((const char *) content_array, env->GetArrayLength(jarray));
    env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
  }
  ValueEvent ev(text);
  mainThread->sendEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_intChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, jint changedInt, jint changedInt2) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "intChangedEvent: %d %d", changedInt, changedInt2);
  ValueEvent ev(changedInt, changedInt2);
  mainThread->sendEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_visibilityChangedEvent(JNIEnv* env, jobject thiz, double timestamp, jint id, bool visible) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "visibilityChangedEvent on %d", id);
  VisibilityEvent ev(visible);
  mainThread->sendEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_nativeAddPreference(JNIEnv* env, jobject thiz, jstring jkey, jstring jvalue) {
  const char * key = env->GetStringUTFChars(jkey, NULL);
  const char * value = env->GetStringUTFChars(jvalue, NULL);
  initialPrefs.setText(key, value);
  env->ReleaseStringUTFChars(jkey, key);
  env->ReleaseStringUTFChars(jvalue, value);
}

void Java_com_sometrik_framework_FrameWork_timerEvent(JNIEnv* env, jobject thiz, double timestamp, jint viewId, jint timerId){
  TimerEvent ev(timerId);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_setNativeActiveView(JNIEnv* env, jobject thiz, double timestamp, jint activeView, bool recordHistory) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "setActivewView: %u", activeView);
  auto & app = mainThread->getPlatform().getApplication();
  if (app.getActiveViewId() != 0 && recordHistory) {
    app.addToHistory(app.getActiveViewId());
  }
  app.setActiveViewId(activeView);
  //TODO
  // Wrong thread. Send event instead
}

void Java_com_sometrik_framework_FrameWork_OnPurchaseEvent(JNIEnv* env, jclass clazz, double timestamp, jint applicationId, jstring productId, bool newPurchase, double purchaseTime){
  const char * cstring = env->GetStringUTFChars(productId, 0);
  //PurchaseEvent Type not used yet. PurchaseTime is not sent //TODO
  PurchaseEvent ev(cstring, PurchaseEvent::PURCHASE_STATUS, newPurchase);
 //Id not used. Sent to 0 //TODO
  mainThread->sendEvent(applicationId, ev);
  env->ReleaseStringUTFChars(productId, cstring);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  return JNI_VERSION_1_6;
}

}
