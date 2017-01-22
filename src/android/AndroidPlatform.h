#include <FWPlatform.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidSoundCanvas.h>
#include <AndroidLogger.h>
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <gl.h>

#include <EventQueue.h>

class JavaCache {

public:
  JavaCache(JNIEnv * env) {
    env->GetJavaVM(&javaVM);

    nativeCommandClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/NativeCommand"));
    frameworkClass = (jclass) env->NewGlobalRef(env->FindClass("com/sometrik/framework/FrameWork"));
    systemClass = (jclass) env->NewGlobalRef(env->FindClass("java/lang/System"));

    loadPrefsValueMethod = env->GetMethodID(frameworkClass, "addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V");
    nativeCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIII[B[BI)V");
    sendCommandMethod = env->GetStaticMethodID(frameworkClass, "sendMessage", "(Lcom/sometrik/framework/FrameWork;Lcom/sometrik/framework/NativeCommand;)V");

    if (env->ExceptionCheck()){

    }
  }

  ~JavaCache() {
    JNIEnv * env = getJNIEnv();
      env->DeleteGlobalRef(nativeCommandClass);
      env->DeleteGlobalRef(frameworkClass);
  }

  JNIEnv * getJNIEnv() {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Canvas getJNIENv called");
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
  jmethodID sendCommandMethod;

private:
  JavaVM * javaVM;
};

class AndroidPlatform: public FWPlatform {
public:
 AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, JavaVM * _javaVM)
   : FWPlatform(_display_scale),
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
    return std::unique_ptr<canvas::ContextFactory>(new canvas::AndroidContextFactory>(mgr, canvasCache, getDisplayScale()));
  }

  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new AndroidClientFactory(clientCache));
  }
  void createFBO(int flags) { }
  std::string showTextEntryDialog(const std::string & title, const std::string & message) { return ""; }

#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    return std::make_shared<AndroidSoundCanvas>(soundCache);
  }
#endif

  std::shared_ptr<Logger> createLogger(const std::string & name) const override {
    return std::make_shared<AndroidLogger>(name);
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
    queueEvent(0, ev);
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

  ANativeWindow * window = 0;
  JavaVM * gJavaVM = 0;
  jobject mgr;
  jobject framework;
  jobject handler;
  EventQueue eventqueue;
  bool canDraw = false, isPaused = false, isDestroyed = false;
};
