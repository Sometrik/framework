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
    addPrefsValueMethod  = env->GetMethodID(frameworkClass, "getFromPrefs", "(Ljava/lang/String;)Ljava/lang/String;");
    currentTimeMillisMethod = env->GetStaticMethodID(systemClass, "currentTimeMillis", "()J");
    nativeCommandConstructor = env->GetMethodID(nativeCommandClass, "<init>", "(Lcom/sometrik/framework/FrameWork;IIIILjava/lang/String;Ljava/lang/String;)V");
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
  jmethodID addPrefsValueMethod;
  jmethodID storeValueMethod;
  jmethodID currentTimeMillisMethod;
  jmethodID nativeCommandConstructor;
  jmethodID sendCommandMethod;

private:
  JavaVM * javaVM;
};

class AndroidPlatform: public FWPlatform {
public:
  AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
      FWPlatform(_display_scale, _glsl_version, _has_es3), javaCache(JavaCache(_env)) {
    framework = _env->NewGlobalRef(_framework);
    mgr = _env->NewGlobalRef(_mgr);
  }
  ~AndroidPlatform() {
  }
  
  void setJavaVM(JavaVM * _gJavaVM) { gJavaVM = _gJavaVM; }
  JavaVM * getJavaVM() { return gJavaVM; }

  std::string getBundleFilename(const char * filename) override;
  std::string getLocalFilename(const char * filename, FileType type) override;
  double getTime() const override;
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::make_shared<canvas::AndroidContextFactory>(getJNIEnv(), mgr);
  }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    auto env = getJNIEnv();
    return std::make_shared < AndroidClientFactory > (env);
  }
  void createFBO(int flags) { }
  std::string showTextEntryDialog(const std::string & message) { return ""; }

#ifdef HAS_SOUNDCANVAS
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    auto env = getJNIEnv();
    return std::make_shared<AndroidSoundCanvas>(env, mgr);
  }
}
#endif

  void storeValue(const std::string & key, const std::string & value) override;
  std::string loadValue(const std::string & key) override;
  void showCanvas(canvas::ContextAndroid & context);
  std::shared_ptr<Logger> createLogger() const override {
    return std::make_shared<AndroidLogger>();
  }
  void sendCommand(const Command & command) override;
  bool initializeRenderer(ANativeWindow * _window);
  void deinitializeRenderer();
  void startThread();
  void renderLoop();
  static void* threadStartCallback(void *myself);

  JNIEnv* getJNIEnv() const;
//  JavaCache getJavaCache() const { return &javaCache; }

  void queueEvent(int internal_id, EventBase & ev) {
#ifdef USE_NATIVE_SURFACE
    eventqueue.push(internal_id, ev);
#else
    postEvent(internal_id, ev);
#endif
  }

  void swapBuffers() override;

private:
  pthread_t _threadId;
  JavaCache javaCache;

  EGLDisplay display = 0;
  EGLSurface surface = 0;
  EGLContext context = 0;

  ANativeWindow * window = 0;
  JavaVM * gJavaVM = 0;
  jobject mgr;
  jobject framework;
  jobject handler;
  EventQueue eventqueue;
  int runloop_level = 0;
  bool renderingEnabled = true;


};
