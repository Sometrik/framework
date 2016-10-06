#include <FWPlatformBase.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidSoundCanvas.h>
class shader_program;

class AndroidPlatform: public FWPlatformBase {


public:
  AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
      FWPlatformBase(_display_scale, _glsl_version, _has_es3) {
    framework = _env->NewGlobalRef(_framework);
    mgr = _env->NewGlobalRef(_mgr);
  }
  ~AndroidPlatform() {
  }

  bool onTouchesEvent(jobject * _obj, int mode, int fingerIndex, double time, float x, float y);

  void onResize(int width, int height);

  void menuPressed();

  bool onUpdate(double timestamp);

  void onDraw();

  void createInputDialog(const char * _title, const char * _message, int params);

  void onInit(JNIEnv * env, JavaVM * _gJavaVM);

  void createOptions();

  void messagePoster(int message, const std::string text);
  void messagePoster(int message, const std::string title, const std::string text);

  void settingsCreator(jobject settings, jint menuId);

  void playSound(jobject sound);

  void stopSound(jobject sound);

  bool createWindow(FWContextBase * context, const char * title) override {
    return false;
  }
  ;
  void showMessageBox(const std::string & title, const std::string & message) override;

  std::string showTextEntryDialog(const std::string & message) override;
  void postNotification(const std::string & title, const std::string & message) override;
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
  void launchBrowser(const std::string & input_url) override;
  void storeValue(const std::string & key, const std::string & value) override;
  std::string loadValue(const std::string & key) override;
  int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) override;
  void showCanvas(canvas::ContextAndroid & context);

  JNIEnv* getJNIEnv() const;
  void setupLooper();

private:
  JavaVM * gJavaVM;
  FWContextBase * application;
  jobject mgr;
  jobject framework;
  jobject handler;
  char message[256];
  shader_program * test_program = 0;
};
