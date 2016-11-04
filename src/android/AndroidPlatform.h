#include <FWPlatform.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidSoundCanvas.h>
#include <AndroidLogger.h>

class AndroidPlatform: public FWPlatform {
public:
  AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
      FWPlatform(_display_scale, _glsl_version, _has_es3) {
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

  void messagePoster(int message, const std::string text);
  void messagePoster(int message, const std::string title, const std::string text);
  void messagePoster(int message, int content);

  void showMessageBox(const std::string & title, const std::string & message) override;

  std::string showTextEntryDialog(const std::string & message) override;
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
  void createFormView(int id){
    messagePoster(8, id);
  }
  void createOpenGLView(int id){
    messagePoster(9, id);
  }
  void showView(int id){
    messagePoster(7, id);
  }

  void sendMessage(const Message & message) override;

  JNIEnv* getJNIEnv() const;

private:
  JavaVM * gJavaVM;
  jobject mgr;
  jobject framework;
  jobject handler;
  char message[256];
};
