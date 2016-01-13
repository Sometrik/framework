#include <string.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include "Menu.h"
#include "program.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include "VBO.h"
#include <glm/gtc/matrix_transform.hpp>
#include <android/bitmap.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>

#define TAG "CubeWallpaper1.c"

using namespace gpufw;
using namespace std;

float touchX = 200.0f;
float touchY = 200.0f;
int screenWidth = 100;
int screenHeight = 100;

static jclass myClass;

std::shared_ptr<AndroidPlatform> platform;

class AndroidPlatform {

public:
	AndroidPlatform(JNIEnv * _env, jobject * _framework) :
			env(_env), framework(_framework) {
	}
	~AndroidPlatform() {
	}

	bool onTouchesEvent(jobject * _obj, int mode, int fingerIndex, long time, float x, float y) override

	void onResize(int width, int height) override

	void menuPressed() override

	bool update() override

	void onDraw() override

	int createMessageDialog(const char * _title, const char * _message, int params) override

	void createInputDialog(const char * _title, const char * _message, int params) override

	void loadImage(jobject bitmap) override

	void showCanvas(jobject canvasBitmap, jobject surface) override

	void onInit(jobject assetManager, jobject surface) override

	void createOptions() override

	void messagePoster(int message) override

	void settingsCreator(jobject thiz, jint menuId) override

	void createSound(jobject thiz) override

	void playSound(jobject sound) override

	void stopSound(jobject sound) override

private:
	JNIEnv * env;
	jobject * framework;
	char message[256];
};
