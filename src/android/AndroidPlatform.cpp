/**
 * Copyright (C) Sometrik Oy 2015 - 2017
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
#include <ImageRequestEvent.h>
#include <ScrollChangedEvent.h>
#include <InternalFormat.h>

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

    nativeCommandTransactionClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/NativeCommandTransaction"));
    nativeCommandClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/NativeCommand"));
    frameworkClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/FrameWork"));
    bitmapClass = (jclass) env->NewGlobalRef(env->FindClass("android/graphics/Bitmap"));
    byteBufferClass = (jclass) env->NewGlobalRef(env->FindClass("java/nio/ByteBuffer"));

    nativeCommandTransactionConstructor = env->GetMethodID(nativeCommandTransactionClass, "<init>", "()V");
    addCommandMethod = env->GetMethodID(nativeCommandTransactionClass, "addCommand", "(Lcom/sometrik/framework/NativeCommand;)V");
    nativeCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BI)V");
    nativeListCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BIIIIII)V");
    sendTransactionMethod = env->GetStaticMethodID(frameworkClass, "sendTransaction", "(Lcom/sometrik/framework/FrameWork;Lcom/sometrik/framework/NativeCommandTransaction;)V");
    sendBitmapMethod = env->GetStaticMethodID(frameworkClass, "sendBitmap", "(Lcom/sometrik/framework/FrameWork;ILandroid/graphics/Bitmap;)V");
    bitmapCreateMethod = env->GetStaticMethodID(bitmapClass, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    copyPixelsFromBufferMethod = env->GetMethodID(bitmapClass, "copyPixelsFromBuffer", "(Ljava/nio/Buffer;)V");
    wrapMethod = env->GetStaticMethodID(byteBufferClass, "wrap", "([B)Ljava/nio/ByteBuffer;");
    
    jclass bitmapConfigClass = (jclass)env->FindClass("android/graphics/Bitmap$Config");
    jfieldID field_argb_4444 = env->GetStaticFieldID(bitmapConfigClass, "ARGB_4444", "Landroid/graphics/Bitmap$Config;");
    jfieldID field_argb_8888 = env->GetStaticFieldID(bitmapConfigClass, "ARGB_8888", "Landroid/graphics/Bitmap$Config;");
    jfieldID field_rgb_565 = env->GetStaticFieldID(bitmapConfigClass, "RGB_565", "Landroid/graphics/Bitmap$Config;");
    
    config_rgba_4444 = env->NewGlobalRef(env->GetStaticObjectField(bitmapConfigClass, field_argb_4444));
    config_rgba_8888 = env->NewGlobalRef(env->GetStaticObjectField(bitmapConfigClass, field_argb_8888));
    config_rgb_565 = env->NewGlobalRef(env->GetStaticObjectField(bitmapConfigClass, field_rgb_565));

    env->DeleteLocalRef(bitmapConfigClass);
      
    if (env->ExceptionCheck()){

    }
  }

  ~JavaCache() {
#if 0
    env->DeleteGlobalRef(framework);
    env->DeleteGlobalRef(nativeCommandClass);
    env->DeleteGlobalRef(frameworkClass);
#endif
  }

  JavaVM & getJavaVM() { return *javaVM; }

  JavaVM * javaVM = 0;

  jobject framework;
  jobject config_rgb_565;
  jobject config_rgba_4444;
  jobject config_rgba_8888;
  
  jclass nativeCommandTransactionClass;
  jclass nativeCommandClass;
  jclass frameworkClass;
  jclass bitmapClass;
  jclass byteBufferClass;

  jmethodID nativeCommandTransactionConstructor;
  jmethodID addCommandMethod;
  jmethodID nativeCommandConstructor;
  jmethodID nativeListCommandConstructor;
  jmethodID sendTransactionMethod;
  jmethodID sendBitmapMethod;
  jmethodID bitmapCreateMethod;
  jmethodID copyPixelsFromBufferMethod;
  jmethodID wrapMethod;
};

class AndroidThread;
class AndroidMainThread;

extern FWApplication * applicationMain();

class AndroidThread : public PosixThread {
public:
  AndroidThread(PlatformThread * _parent_thread, FWPlatform * _platform, std::shared_ptr<FWApplication> & _application, std::shared_ptr<JavaCache> & _cache, AAssetManager * _asset_manager, std::shared_ptr<Runnable> & _runnable)
    : PosixThread(_parent_thread, _platform, _application, _runnable), javaCache(_cache), asset_manager(_asset_manager) {
  }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new AndroidClientFactory);
  }
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::AndroidContextFactory(getAssetManager(), getDisplayScale()));
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

  std::shared_ptr<PlatformThread> createThread(std::shared_ptr<Runnable> & runnable) override {
    return std::make_shared<AndroidThread>(this, &(getPlatform()), application, javaCache, asset_manager, runnable);
  }

  void setImageData(int internal_id, std::shared_ptr<canvas::PackedImageData> image) override {
    jobject config = 0;
    switch (image->getInternalFormat()) {
    case canvas::InternalFormat::RGB565:
      config = javaCache->config_rgb_565;
      break;
    case canvas::InternalFormat::RGBA4:
      config = javaCache->config_rgba_4444;
      break;
    case canvas::InternalFormat::RGBA8:
      config = javaCache->config_rgba_8888;
      break;
    }

    if (!config) return;

    const jbyte * bytePtr = reinterpret_cast<const jbyte*>(image->getData());
    size_t size = image->calculateSizeForFirstLevel();
    jbyteArray bytes = myEnv->NewByteArray(size);
    myEnv->SetByteArrayRegion(bytes, 0, size, bytePtr);

    jobject buffer = myEnv->CallStaticObjectMethod(javaCache->byteBufferClass, javaCache->wrapMethod, bytes);
    jobject bitmap = myEnv->CallStaticObjectMethod(javaCache->bitmapClass, javaCache->bitmapCreateMethod, (int)image->getWidth(), (int)image->getHeight(), config);

    myEnv->CallVoidMethod(bitmap, javaCache->copyPixelsFromBufferMethod, buffer);
    myEnv->CallStaticVoidMethod(javaCache->frameworkClass, javaCache->sendBitmapMethod, javaCache->framework, internal_id, bitmap);

    myEnv->DeleteLocalRef(bitmap);
    myEnv->DeleteLocalRef(buffer);
    myEnv->DeleteLocalRef(bytes);   
  }

  int sendCommands(const vector<Command> & commands) override {
    bool startLoop = false;

    jobject jtransaction = myEnv->NewObject(javaCache->nativeCommandTransactionClass, javaCache->nativeCommandTransactionConstructor);

    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "SENDING TRANSACTION (%d)", (int)commands.size());

    for (auto & command : commands) {
      if (command.getType() == Command::CREATE_FRAMEVIEW || command.getType() == Command::CREATE_OPENGL_VIEW) {
	auto & app = getApplication();
	if (!app.getActiveViewId()) {
	  app.setActiveViewId(command.getChildInternalId());
	}
      } else if (command.getType() == Command::SHOW_MODAL) {
        startLoop = true;
      } else if (command.getType() == Command::END_MODAL) {
        exit_loop = true;
      }
      
      int commandTypeId = int(command.getType());
      auto & textValue = command.getTextValue();
      auto & textValue2 = command.getTextValue2();
      jbyteArray jtextValue = 0, jtextValue2 = 0;
      if (!textValue.empty()) jtextValue = convertToByteArray(myEnv, textValue);
      if (!textValue2.empty()) jtextValue2 = convertToByteArray(myEnv, textValue2);
      
      jobject jcommand = myEnv->NewObject(javaCache->nativeCommandClass, javaCache->nativeListCommandConstructor, javaCache->framework, commandTypeId, command.getInternalId(), command.getChildInternalId(), command.getValue(), jtextValue, jtextValue2, command.getFlags(), command.getRow(), command.getColumn(), command.getSheet(), command.getWidth(), command.getHeight());
      myEnv->CallVoidMethod(jtransaction, javaCache->addCommandMethod, jcommand);
      
      myEnv->DeleteLocalRef(jcommand);
      if (jtextValue) myEnv->DeleteLocalRef(jtextValue);
      if (jtextValue2) myEnv->DeleteLocalRef(jtextValue2);
    }

    myEnv->CallStaticVoidMethod(javaCache->frameworkClass, javaCache->sendTransactionMethod, javaCache->framework, jtransaction);
    myEnv->DeleteLocalRef(jtransaction);

    if (startLoop) {
      setModalResultValue(0);
      startEventLoop();
      return getModalResultValue();
    } else {
      return 0;
    }
  }

  AAssetManager * getAssetManager() const { return asset_manager; }

protected:
  void initializeThread() override {
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup

    javaCache->getJavaVM().AttachCurrentThread(&myEnv, &args);
  }
  void deinitializeThread() override {
    javaCache->getJavaVM().DetachCurrentThread();
  }

  JNIEnv * myEnv = 0;
  std::shared_ptr<JavaCache> javaCache;
  AAssetManager * asset_manager;
  bool exit_loop = false;
};

class AndroidMainThread : public AndroidThread {
public:
  AndroidMainThread(std::shared_ptr<FWApplication> & _application, std::shared_ptr<JavaCache> & _cache, AAssetManager * _asset_manager, std::shared_ptr<Runnable> & _runnable)
  : AndroidThread(nullptr, new FWPlatform, _application, _cache, _asset_manager, _runnable) { }

  void startRunnable() override {
    application->initialize(this);
    application->initializeChildren();

    startEventLoop();
    deinitializeRenderer();
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
      AndroidThread::onOpenGLInitEvent(_ev);
    }
  }

  void onSysEvent(SysEvent & ev) override {
    PlatformThread::onSysEvent(ev);

    if (ev.getType() == SysEvent::PAUSE) {
      isPaused = true;
    } else if (ev.getType() == SysEvent::RESUME) {
      isPaused = false;
    } else if (ev.getType() == SysEvent::DESTROY) {
      isDestroyed = true;
    }
  }

  void onResizeEvent(ResizeEvent & ev) override {
    setActualDisplayWidth(ev.getActualWidth());
    setActualDisplayHeight(ev.getActualHeight());
  }

  bool initializeRenderer(int opengl_es_version, ANativeWindow * _window) {
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

  void deinitializeRenderer() {
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

  void startEventLoop() override {
    exit_loop = false;
    
    while (!isDestroyed && !exit_loop) {
      auto evs = pollEvents();

      bool redraw = false, update_sent = false;
      for (auto & ev : evs) {
        if (dynamic_cast<UpdateEvent*>(ev.second.get())) {
          if (update_sent) continue;
          update_sent = true;
        }

        getPlatform().postEvent(ev.first, *ev.second.get());

        auto ev2 = dynamic_cast<SysEvent*>(ev.second.get());
        if (ev2) {
          if (ev2->getType() == SysEvent::DESTROY) {
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

private:
  EGLDisplay display = 0;
  EGLSurface surface = 0;
  EGLContext context = 0;

  ANativeWindow * window = 0;
  bool canDraw = false, isPaused = false, isDestroyed = false;

  MobileAccount mobileAccount;
  FWPreferences preferences;
};

static std::shared_ptr<AndroidMainThread> mainThread;
static FWPreferences initialPrefs;

#ifdef PROFILING
extern "C" void monstartup( char const * );
extern "C" void moncleanup();
#endif
extern "C" {

void Java_com_sometrik_framework_FrameWork_onResize(JNIEnv* env, jclass clazz, float x, float y, int viewId) {
  ResizeEvent ev(x / mainThread->getDisplayScale(), y / mainThread->getDisplayScale(), x, y);
  mainThread->sendEvent(mainThread->getInternalId(), ev);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_keyPressed(JNIEnv* env, jobject thiz, int keyId, int viewId) {
  auto & application = mainThread->getApplication();
  if (keyId == 4) {
    SysEvent ev(SysEvent::BACK);
    mainThread->sendEvent(application.getInternalId(), ev);
  } else if (keyId == 82) {
    CommandEvent ce(FW_ID_MENU);
    mainThread->sendEvent(application.getActiveViewId(), ce);
  } else {
    CommandEvent ce(keyId);
    mainThread->sendEvent(viewId, ce);
  }
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int viewId, int mode, int fingerIndex, double timestamp, float x, float y) {
  x /= mainThread->getDisplayScale();
  y /= mainThread->getDisplayScale();
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
    const char * email = env->GetStringUTFChars(jemail, NULL);
    const char * language = env->GetStringUTFChars(jlanguage, NULL);
    const char * country = env->GetStringUTFChars(jcountry, NULL);

    MobileAccount account = MobileAccount(email, language, country);

    env->ReleaseStringUTFChars(jlanguage, language);
    env->ReleaseStringUTFChars(jemail, email);
    env->ReleaseStringUTFChars(jcountry, country);

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    AndroidClientFactory::initialize(env);
    canvas::AndroidContextFactory::initialize(env, assetManager);

    initialPrefs.clearChanges();

    shared_ptr<FWApplication> application(applicationMain());
    application->setPreferences(initialPrefs);
    application->setMobileAccount(account);

    auto cache = make_shared<JavaCache>(env, thiz);

    shared_ptr<Runnable> runnable;

    mainThread = make_shared<AndroidMainThread>(application, cache, manager, runnable);
    mainThread->setActualDisplayWidth(screenWidth);
    mainThread->setActualDisplayHeight(screenHeight);
    mainThread->setDisplayScale(displayScale);

//    setenv("CPUPROFILE", "/data/data/com.sometrik.kraphio/files/gmon.out", 1);
#ifdef PROFILING
    monstartup("framework.so");
#endif

    mainThread->start();
  }
}

void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface, int surfaceId, int gl_version, int width, int height) {
  if (surface != 0) {
    ResizeEvent ev0(int(width / mainThread->getDisplayScale()), int(height / mainThread->getDisplayScale()), width, height);
    mainThread->sendEvent(mainThread->getInternalId(), ev0);
  
    ANativeWindow * window = 0;
    window = ANativeWindow_fromSurface(env, surface);
    AndroidOpenGLInitEvent ev(gl_version, true, window);
    mainThread->sendEvent(mainThread->getInternalId(), ev);
    mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
  }
}

 void Java_com_sometrik_framework_FrameWork_nativeSurfaceDestroyed(JNIEnv* env, jobject thiz, int surfaceId, int gl_version) {
   mainThread->deinitializeRenderer();
 }

void Java_com_sometrik_framework_FrameWork_nativeOnResume(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(SysEvent::RESUME);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnPause(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(SysEvent::PAUSE);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStop(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(SysEvent::STOP);
  mainThread->sendEvent(appId, ev);
}
void Java_com_sometrik_framework_FrameWork_nativeOnStart(JNIEnv* env, jobject thiz, int appId) {
  if (mainThread.get()) {
    SysEvent ev(SysEvent::START);
    mainThread->sendEvent(appId, ev);
  } else {
    __android_log_print(ANDROID_LOG_INFO, "Sometrik", "NO THREAD FOR SysEvent::START");
  }
}
void Java_com_sometrik_framework_FrameWork_nativeOnDestroy(JNIEnv* env, jobject thiz, int appId) {
  mainThread->terminate();
  SysEvent ev(SysEvent::DESTROY);
  mainThread->sendEvent(appId, ev);
  // TODO: Wait for threads to terminate
#ifdef PROFILING
  moncleanup();
#endif

  // mainThread = shared_ptr<AndroidMainThread>(0);
}
void Java_com_sometrik_framework_FrameWork_languageChanged(JNIEnv* env, jobject thiz, int appId, jstring language) {
  const char * clanguage = env->GetStringUTFChars(language, NULL);
  SysEvent ev(SysEvent::LANGUAGE_CHANGED);
  ev.setTextValue(clanguage);
  mainThread->sendEvent(appId, ev);
  env->ReleaseStringUTFChars(language, clanguage);
}
void Java_com_sometrik_framework_FrameWork_memoryWarning(JNIEnv* env, jobject thiz, int appId) {
  SysEvent ev(SysEvent::MEMORY_WARNING);
  mainThread->sendEvent(appId, ev);
}
  
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, jint id, jbyteArray jarray) {
  string text;
  if (jarray) {
    jbyte* content_array = env->GetByteArrayElements(jarray, NULL);
    text = string((const char *) content_array, env->GetArrayLength(jarray));
    env->ReleaseByteArrayElements(jarray, content_array, JNI_ABORT);
  }
  ValueEvent ev(text);
  mainThread->sendEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_intChangedEvent(JNIEnv* env, jobject thiz, jint id, jint changedInt, jint changedInt2) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "intChangedEvent: %d %d", changedInt, changedInt2);
  ValueEvent ev(changedInt, changedInt2);
  mainThread->sendEvent(id, ev);
}

void Java_com_sometrik_framework_FrameWork_visibilityChangedEvent(JNIEnv* env, jobject thiz, jint id, bool visible) {
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

void Java_com_sometrik_framework_FrameWork_nativeScrollChanged(JNIEnv * env, jobject thiz, int viewId, int scrollPos, int scrollRem, int height) {
  ScrollChangedEvent ev(scrollPos, scrollRem, height);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_sendImageRequest(JNIEnv* env, jobject thiz, jint viewId, jstring uri, jint width, jint height, jint internalFormat) {
  const char * uri2 = env->GetStringUTFChars(uri, NULL);
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "sendImageRequest: %s %d %d", uri2, width, height);
  ImageRequestEvent ev(ImageRequestEvent::REQUEST, viewId, uri2, ImageRequestEvent::NORMAL, width, height);
  if (internalFormat != 0) {
    ev.setInternalFormat((canvas::InternalFormat)internalFormat);
  }
  mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
  env->ReleaseStringUTFChars(uri, uri2);
}

void Java_com_sometrik_framework_FrameWork_cancelImageRequest(JNIEnv * env, jobject thiz, jint viewId) {
  ImageRequestEvent ev(ImageRequestEvent::CANCEL, viewId);
  mainThread->sendEvent(mainThread->getApplication().getInternalId(), ev);
}
void Java_com_sometrik_framework_FrameWork_timerEvent(JNIEnv* env, jobject thiz, jint viewId, jint timerId) {
  TimerEvent ev(timerId);
  mainThread->sendEvent(viewId, ev);
}

void Java_com_sometrik_framework_FrameWork_setNativeActiveView(JNIEnv* env, jobject thiz, jint activeView, bool recordHistory) {
  __android_log_print(ANDROID_LOG_INFO, "Sometrik", "setActivewView: %u", activeView);
  auto & app = mainThread->getApplication();
  if (app.getActiveViewId() != 0 && recordHistory) {
    app.addToHistory(app.getActiveViewId());
  }
  app.setActiveViewId(activeView);
  //TODO
  // Wrong thread. Send event instead
}

void Java_com_sometrik_framework_FrameWork_OnPurchaseEvent(JNIEnv* env, jclass clazz, jint applicationId, jstring productId, bool newPurchase, double purchaseTime){
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
