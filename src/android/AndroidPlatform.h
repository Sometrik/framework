#include <FWPlatformBase.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
class shader_program;

class AndroidPlatform : public FWPlatformBase {

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

	int createMessageDialog(const char * _title, const char * _message, int params);

	void createInputDialog(const char * _title, const char * _message, int params);

	void onInit(jobject surface);

	void createOptions();

	void messagePoster(int message);

	void settingsCreator(jobject thiz, jint menuId);

	void playSound(jobject sound);

	void stopSound(jobject sound);

   bool createWindow(FWContextBase * context, const char * title){ return false; };
   void showMessageBox(const std::string & message) {
  	 createMessageDialog("", message.c_str(), 0);
   }
  std::string showTextEntryDialog(const std::string & message) { }
  void postNotification(const std::string & message) { }
  std::string getBundleFilename(const char * filename) { return ""; }
  std::string getLocalFilename(const char * filename) { return ""; }
  std::shared_ptr<canvas::ContextFactory> createContextFactory() const override { return std::make_shared<canvas::AndroidContextFactory>(env, mgr); }
  std::shared_ptr<HTTPClientFactory> createHTTPClientFactory() const override { return std::make_shared<AndroidClientFactory>(env); }
  void launchBrowser(const std::string & input_url) { }
  void storeValue(const std::string & key, const std::string & value) { }
  std::string loadValue(const std::string & key) { return ""; }
  int showActionSheet(const FWRect & rect, const FWActionSheet & sheet) { }

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
