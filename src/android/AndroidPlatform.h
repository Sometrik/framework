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

class AndroidPlatform: public FWPlatform {
public:
  AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
      FWPlatform(_display_scale, _glsl_version, _has_es3), display(0), surface(0), context(0), angle(0) {
    framework = _env->NewGlobalRef(_framework);
    mgr = _env->NewGlobalRef(_mgr);
  }
  ~AndroidPlatform() {
  }
  
  void setJavaVM(JavaVM * _gJavaVM) { gJavaVM = _gJavaVM; }

  std::string getBundleFilename(const char * filename) override;
  std::string getLocalFilename(const char * filename, FileType type) override;
  double getTime() const override;
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::make_shared<canvas::AndroidContextFactory>(getJNIEnv(), mgr);
  }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    auto env = getJNIEnv();
    return std::make_shared<AndroidClientFactory>(env);
  }
  std::shared_ptr<SoundCanvas> createSoundCanvas() const override {
    auto env = getJNIEnv();
    return std::make_shared<AndroidSoundCanvas>(env, mgr);
  }
  void storeValue(const std::string & key, const std::string & value) override;
  std::string loadValue(const std::string & key) override;
  int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) override;
  void showCanvas(canvas::ContextAndroid & context);
  std::shared_ptr<Logger> createLogger() const override {
    return std::make_shared<AndroidLogger>();
  }
  void sendMessage(const Message & message) override;
  bool initializeRenderer(ANativeWindow * _window);
  void startThread();
  void renderLoop();
  static void* threadStartCallback(void *myself);

  JNIEnv* getJNIEnv() const;

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

  EGLDisplay display = 0;
  EGLSurface surface = 0;
  EGLContext context = 0;

  ANativeWindow * window = 0;
  JavaVM * gJavaVM = 0;
  jobject mgr;
  jobject framework;
  jobject handler;
  char message[256];
  EventQueue eventqueue;
  int runloop_level = 0;
  bool renderingEnabled = true;
};
