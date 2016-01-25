#include <FWPlatformBase.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
class shader_program;

class AndroidPlatform : FWPlatformBase {

public:
   AndroidPlatform(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
		FWPlatformBase(_display_scale, _glsl_version, _has_es3), env(_env), mgr(_mgr), framework(_framework) {
	}
	~AndroidPlatform() {
	}

	bool onTouchesEvent(jobject * _obj, int mode, int fingerIndex, long time, float x, float y);

	void onResize(int width, int height);

	void menuPressed();

	bool update();

	void onDraw();

	void createInputDialog(const char * _title, const char * _message, int params);

	void onInit(jobject surface);

	void createOptions();

	void messagePoster(int message, const std::string text);

	void settingsCreator(jobject thiz, jint menuId);

	void playSound(jobject sound);

	void stopSound(jobject sound);

   bool createWindow(FWContextBase * context, const char * title) override { return false; };
   void showMessageBox(const std::string & title, const std::string & message) override;

  std::string showTextEntryDialog(const std::string & message) override;
  void postNotification(const std::string & message) override;
  std::string getBundleFilename(const char * filename) override { return ""; }
  std::string getLocalFilename(const char * filename, FileType type) override {
  	switch (type) {
  	case DATABASE: return "";
  	case NORMAL: return "";
  	}
 }
  double getTime() const override;
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override { return std::make_shared<canvas::AndroidContextFactory>(env, mgr); }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override { return std::make_shared<AndroidClientFactory>(env); }
  void launchBrowser(const std::string & input_url) override;
  void storeValue(const std::string & key, const std::string & value) override { }
  std::string loadValue(const std::string & key) override { return ""; }
  int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) override { }


//protected:
	//void showCanvas(jobject canvasBitmap, jobject surface);
	//void loadImage(jobject bitmap);
	//void createSound(jobject thiz);

private:
	JNIEnv * env;
	jobject mgr;
	jobject framework;
	char message[256];
	shader_program * test_program = 0;
};
