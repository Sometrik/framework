/*
 * Copyright (C) Sometrik oy 2015
 *
 */

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
#include <AndroidPlatform.h>
#include <shader_program.h>

#define TAG "CubeWallpaper1.c"

using namespace gpufw;
using namespace std;

float touchX = 200.0f;
float touchY = 200.0f;
int screenWidth = 100;
int screenHeight = 100;

static jclass myClass;

bool
AndroidPlatform::onTouchesEvent(jobject * _obj, int mode, int fingerIndex, long time, float x, float y) {
	//Palauttaa vastauksen threadille (debug)
 // messagePoster(11);
	touchX = x;
	touchY = y;

	return true;
}

void
AndroidPlatform::onResize(int width, int height) {

		__android_log_print(ANDROID_LOG_ERROR, "Sometrik", "resize: %d %d ", width, height);

		screenWidth = width;
		screenHeight = height;

	}
void
AndroidPlatform::menuPressed() {

	//--------- creating actionSheet for debug
	FWActionSheet debugSheet = FWActionSheet("This is title");
	debugSheet.addOption(1, "Social Media");
	debugSheet.addOption(2, "Application Settings");
	debugSheet.addOption(3, "Login Settings");
	//------------------------

		showActionSheet(FWRect(), debugSheet);
}

int
AndroidPlatform::showActionSheet(const FWRect & rect, const FWActionSheet & sheet){

	//Initialize java int and string arrays
	std::vector<FWOption> optionsVector = sheet.getOptions();
	jobjectArray stringArray = (jobjectArray)env->NewObjectArray(optionsVector.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
	jintArray intArray = env->NewIntArray(optionsVector.size());
	jint fill[optionsVector.size()];

	//Set values to java arrays
	for (int i = 0; i < optionsVector.size(); i++){
		env->SetObjectArrayElement(stringArray, i, env->NewStringUTF(optionsVector[i].getText().c_str()));
		fill[i] = optionsVector[i].getId();
	}
	env->SetIntArrayRegion(intArray, 0, optionsVector.size(), fill);

	//Send values to java to create the action sheet
	jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
	jmethodID methodRef = env->GetStaticMethodID(cls, "createOptionsFromJNI", "(Lcom/sometrik/framework/MyGLSurfaceView;I[I[Ljava/lang/String;)V");
	env->CallStaticVoidMethod(cls, methodRef, framework, 22, intArray, stringArray);

	//Not returning anything
	return 0;
}

bool
AndroidPlatform::update() {

		return false;
}

void
AndroidPlatform::onDraw() {

		glClearColor(0.98f, 0.0f, 0.98f, 1.0f);
		glClear (GL_COLOR_BUFFER_BIT);
#if 0
		glm::mat4 projMat = glm::ortho(0.0f, float(screenWidth), 0.0f,
				float(screenHeight), -1000.0f, +1000.0f);
		glm::mat4 mat(1.0f);

		test_program->use();
		test_program->setUniform("proj_matrix", projMat);
		test_program->setUniform("mv_matrix", mat);
		test_program->setUniform("s_texture", 0);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

		VBO vbo;get
		vbo.quad2d(touchX + 10.0f, touchY + 10.0f, touchX + 100.0f,
				touchY + 10.0f, touchX + 100.0f, touchY + 100.0f,
				touchX + 10.0f, touchY + 100.0f);
		vbo.draw();

		glBindTexture(GL_TEXTURE_2D, 0);

		test_program->unuse();
#endif
	}

std::string
AndroidPlatform::showTextEntryDialog(const std::string & message){

}

void
AndroidPlatform::showMessageBox(const std::string & title, const std::string & message){

	messagePoster(5, title, message);
}


void
AndroidPlatform::createInputDialog(const char * _title, const char * _message, int params) {

		jclass cls = env->FindClass("com/sometrik/framework/FrameWork");

		jmethodID methodRef = env->GetMethodID(cls, "createInputDialog", "(Ljava/lang/String;Ljava/lang/String;)V");
		jstring title = env->NewStringUTF(_title);
		jstring message = env->NewStringUTF(_message);

		//Call method with void return (env, object, method, parameters...)
		//String has to be made with jstring name = (*env)->NewStringUTF(env, "What you want");
		env->CallVoidMethod(framework, methodRef, title, message);

		//message[0] = 0;
		//return message;
	}

	static program * test_program;

 void showCanvas(jobject canvasBitmap, jobject surface, JNIEnv * env) {

		jclass cls = env->GetObjectClass(surface);
		jmethodID methodRef = env->GetMethodID(cls, "setNativeCanvas", "(Landroid/graphics/Bitmap;)V");
		env->CallVoidMethod(surface, methodRef, canvasBitmap);

	}

void
AndroidPlatform::onInit() {

		// AAssetManager* manager = AAssetManager_fromJava(env, mgr);

	//menuPressed();

		canvas::AndroidContextFactory factory(env, mgr);
		auto context = factory.createContext(800, 800, canvas::InternalFormat::RGBA8);
		context->globalAlpha = 1.0f;
		context->font.size = 50;
		context->textBaseline = "top";
		context->textAlign = "left";
		//auto yoSurface = context->createSurface("picture.jpg");
		auto yoSurface = context->createSurface(500,500, canvas::InternalFormat::RGBA8);
		//context->shadowBlur = context->shadowOffsetX = context->shadowOffsetY = 5.0f;
		//context->drawImage(*yoSurface, 120, 120, 400, 400);
		context->fillText("Olen Mikko osaan lukea ja kirjoittaa", 20, 100);

		//showCanvas((dynamic_cast<canvas::AndroidSurface&>(*yoSurface)).getBitmap(), surface);
		//auto context = factory.createContext("picture.jpg");

#if 0
		float x = 10;
				float y = 100;
				float w = 200;
				float h = 200;
				float ra = 10.0;

		context->beginPath();
				  context->moveTo(x+ra, y);
				  context->arcTo(x+w, y,   x+w, y+h, ra);
				  context->arcTo(x+w, y+h, x,   y+h, ra);
				  context->arcTo(x,   y+h, x,   y,   ra);
				  context->arcTo(x,   y,   x+w, y,   ra);

				context->fillStyle = "#ffffff";
				context->shadowBlur = context->shadowOffsetX = context->shadowOffsetY = 5.0f;
				context->fill();
				context->strokeStyle = "#ff0000";
				context->lineWidth = 5.0f;
				context->shadowBlur = context->shadowOffsetX = context->shadowOffsetY = 0.0f;
				context->stroke();
#endif
				//showCanvas((dynamic_cast<canvas::AndroidSurface&>(context->getDefaultSurface())).getBitmap(), surface);
#if 0
		AndroidClientFactory clientFactory(env);
		auto android = clientFactory.createClient("yo", false, false);

		HTTPRequest requ = HTTPRequest(HTTPRequest::GET, "http://i.imgur.com/x37PajU.jpg");
		requ.setFollowLocation(false);
		Authorization autor = Authorization();
		//auto resp = android->request(requ, autor);
		auto res = android->Get("http://i.imgur.com/2tfe9LS.jpg");
		if (res.isSuccess()) {
			auto surfaceee = factory.createSurface((unsigned char*)res.getContent().c_str(), res.getContent().size());
			auto imigi = *surfaceee->createImage();
			//context->drawImage(*surfaceee, 0, 0, 300, 300);
			context->drawImage(imigi, 0, 0, 300, 300);
		}
#endif
		showCanvas((dynamic_cast<canvas::AndroidSurface&>(context->getDefaultSurface())).getBitmap(), framework, env);
	}

void
AndroidPlatform::createOptions() {

#if 0
		SettingsTree tree;
		auto subtree1 = tree.createFolder("Eka alivalikko");
		auto subtree2 = tree.createFolder("Toka alivalikko");
		tree.addInt("int1", "Asetus1", 10);
		subtree1->addBool("bool1", "Asetus2", false);
		auto group1 = tree.addGroup("Social media accounts");
		auto subtree3 = group->createFolder("account1", "Facebook tunnus 1", 1);
		subtree3->addLongLong("int2", "Käyttäjä ID", 314123523526)
		group->createFolder("account2", "Twitter tunnus 1", 2);

		// [My application]
		// int1 = 10
		// [account1]
		// int2 = 314123523526
		// [account2]

		ESActionSheet sheet("Otsikko");
		sheet.addOption(1, "Eka vaihtoehto");
		sheet.addOption(2, "Toka vaihtoehto");
		sheet.addOption(3, "Kolmas vaihtoehto");
		ESRect rect(10, 10, 10, 10);
		showActionSheet(rect, sheet);

		void showActionSheet(const ESRect & rect, const ESActionSheet & sheet) {
			const char * title = sheet.getTitle().c_str();
			auto & options = sheet.getOptions();
			jobjectArray stringArray = (jobjectArray) env->NewObjectArray(options.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
			for (int i = 0; i < options.size(); i++) {
				int id = options[i].getId();
				const char * option_text = options[i].getText().c_str();
				env->SetObjectArrayElement(stringArray, i, env->NewStringUTF(option_text));
			}

		}
#endif


	}

std::string
AndroidPlatform::getBundleFilename(const char * filename){

	jstring path = (jstring)env->CallObjectMethod(framework,
			env->GetMethodID(env->GetObjectClass(framework), "getResourcePath", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(filename));
	std::string result = env->GetStringUTFChars(path, JNI_FALSE);

	return result;
}

std::string
AndroidPlatform::getLocalFilename(const char * filename, FileType type) {

	jstring path;
	std::string result;
	switch (type) {
	case DATABASE:
	case CACHE_DATABASE:
		path = (jstring)env->CallObjectMethod(framework,
				env->GetMethodID(env->GetObjectClass(framework), "getDBPath", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(filename));
		result = env->GetStringUTFChars(path, JNI_FALSE);
		return result;
	case NORMAL: return "";
	}
}

std::string
AndroidPlatform::loadValue(const std::string & key){
	jstring value = (jstring)env->CallObjectMethod(framework,
			env->GetMethodID(env->GetObjectClass(framework), "getFromPrefs", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(key.c_str()));
	std::string result = env->GetStringUTFChars(value, JNI_FALSE);

	return result;
}

void
AndroidPlatform::storeValue(const std::string & key, const std::string & value) {
	env->CallVoidMethod(framework, env->GetMethodID(env->GetObjectClass(framework),
			"addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V"), env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
}


void
AndroidPlatform::messagePoster(int message, const std::string text) {

		jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
		jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/MyGLSurfaceView;ILjava/lang/String;)V");

		env->CallStaticVoidMethod(cls, methodRef, framework, message, env->NewStringUTF(text.c_str()));
	}

void
AndroidPlatform::messagePoster(int message, const std::string title, const std::string text) {

		jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
		jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/MyGLSurfaceView;ILjava/lang/String;Ljava/lang/String;)V");

		env->CallStaticVoidMethod(cls, methodRef, framework, message, env->NewStringUTF(title.c_str()), env->NewStringUTF(text.c_str()));
	}

void
AndroidPlatform::settingsCreator(jobject settings, jint menuId) {

		//Tähän jonkinlainen switchi id:n mukaan, minkälainen preferenssivalikko tehdään.
		// preferenssi on tällä hetkellä mode, id, nimi ja niitä voi luoda settingsin listaan
		//kutsumalla createMenuItem. Tällä hetkellä sitä kutsutaan, joka kerta kun asettaa uuden

	jclass checkCls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
	jmethodID methodReff = env->GetMethodID(checkCls , "checkSettings", "(Lcom/sometrik/framework/Settings;)V");

	env->CallVoidMethod(framework, methodReff, settings);

		jclass cls = env->FindClass("com/sometrik/framework/Settings");

		jmethodID methodRef = env->GetMethodID(cls, "createMenuItem", "(IILjava/lang/String;)V");
		jmethodID methodRef2 = env->GetMethodID(cls, "createMenuItem", "(Ljava/lang/String;ILjava/lang/String;)V");

		jstring name = env->NewStringUTF("Hello java");
		jstring name2 = env->NewStringUTF("Hello java again");
		jstring name3 = env->NewStringUTF("Etunimi Sukunimi");
		jstring media1 = env->NewStringUTF("facebook");
		jstring media2 = env->NewStringUTF("twitter");
		jstring media3 = env->NewStringUTF("instagram");
		jint id = 1;
		jint id2 = 2;
		jint mode = 0;
		jint mode2 = 1;

		//env->CallVoidMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "settingsSkip", "(Lcom/sometrik/framework/Settings;)V"), settings);
		env->CallVoidMethod(settings, methodRef, mode, id, name);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
		env->CallVoidMethod(settings, methodRef, mode2, id2, name2);
#if 0
		env->CallVoidMethod(settings, methodRef2, media2, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media2, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
		env->CallVoidMethod(settings, methodRef2, media3, id2, name3);
#endif
	}

void
AndroidPlatform::postNotification(const std::string & title, const std::string & message){
	messagePoster(4, title, message);
 }

double
AndroidPlatform::getTime() const{
	__android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "getTime called");

	jclass frameClass = env->GetObjectClass(framework);
	double currentTime = env->CallStaticDoubleMethod(frameClass, env->GetStaticMethodID(frameClass, "getTime", "()D"));
	__android_log_print(ANDROID_LOG_INFO, "Sometrik", "currentTime = %d", currentTime);

	return currentTime;
}

	//Mahdollisesti vain debuggia varten
	void createSound(jobject thiz) {

		//SoundEffect* effect = new SoundEffect(env, "test", framework);
		//effect->play();

		//Needs JNIEnv
#if 0
		jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");

		jmethodID methodRef = env->GetMethodID(cls, "createSoundObject", "(Ljava/lang/String;)Lcom/sometrik/framework/Sound;");
		jstring sound = env->NewStringUTF("papertrail");

		jobject soundObject = env->CallObjectMethod(thiz, methodRef, sound);

		playSound(soundObject);
#endif

	}

void
AndroidPlatform::playSound(jobject sound) {

		jclass cls = env->FindClass("com/sometrik/framework/Media");
		jmethodID methodRef = env->GetMethodID(cls, "play", "()V");

		env->CallVoidMethod(sound, methodRef);

	}

void
AndroidPlatform::stopSound(jobject sound) {

		jclass cls = env->FindClass("com/sometrik/framework/Media");
		jmethodID methodRef = env->GetMethodID(cls, "stop", "()V");

		env->CallVoidMethod(sound, methodRef);

	}

void
AndroidPlatform::launchBrowser(const std::string & input_url){
	messagePoster(3, "http://stackoverflow.com/questions/4121450/activitynotfoundexception");
}

#if 0
	class SoundEffect {

	private:
		JNIEnv * env;
		char * name;
		jclass cls;
		jobject * thiz;

	public:
		SoundEffect(JNIEnv * _env, char * _name, jobject * _thiz) :
		env(_env), name(_name), thiz(_thiz) {

			cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");

			//jmethodID methodRef = env->GetMethodID(cls, "createSoundObject",
			//		"(Ljava/lang/String;)V");
		}

		void play() {

			jmethodID methodRef = env->GetMethodID(cls, "playSound",
					"(Ljava/lang/String;)V");
			jstring sound = env->NewStringUTF(name);
			env->CallVoidMethod(*thiz, methodRef, sound);

		}

		void stop() {
			jmethodID methodRef = env->GetMethodID(cls, "playSound",
					"(Ljava/lang/String;)V");

		}

	};
#endif

std::shared_ptr<AndroidPlatform> platform;

extern "C" {
void Java_com_sometrik_framework_FrameWork_NativeOnTouch(JNIEnv* env, jobject thiz, jobject obj) {
#if 1
	//platform.createMessageDialog("Hello Java 2", "This is an error message", 0);
	platform->createInputDialog("Hello Java 2", "Type your fucking password", 0);
#else

	//Get class to use
	jclass cls = (*env)->FindClass(env, "com/sometrik/framework/FrameWork");

	// int button_pressed = createTextDialog("Error", "An error occured", OK_BUTTON | CANCEL_BUTTON);

	//Get method id, so you can call it. (env, class, method name, Signature
	//Singature for string is Ljava/lang/String;
	jmethodID methodRef = (*env)->GetMethodID(env, cls, "createTextDialog",
			"(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	jstring name = (*env)->NewStringUTF(env, "Hello java");
	jstring message = (*env)->NewStringUTF(env, "Type your password");
	jstring button1 = (*env)->NewStringUTF(env, "Cancel");
	jstring button2 = (*env)->NewStringUTF(env, "OK");

	//Call method with void return (env, object, method, parameters...)
	//String has to be made with jstring name = (*env)->NewStringUTF(env, "What you want");
	(*env)->CallVoidMethod(env, obj, methodRef, name, message, button1,
			button2);

	//(*env)->CallObjectMethod(env, thiz, methodRef);

	//possible future need
	//jobject objectRef = (*env)->NewObject(env, cls, methodRef);
	//	jobject result = (*env)->CallObjectMethod(env, thiz, textDialog, "heyyy");
	//jclass cls = (*env)->GetObjectClass(env, thiz);
#endif
}

void Java_com_sometrik_framework_MyGLSurfaceView_playSound(JNIEnv* env, jobject thiz, jobject sound) {
	platform->playSound(sound);
}

void Java_com_sometrik_framework_MyGLSurfaceView_stopSound(JNIEnv* env, jobject thiz, jobject sound) {
	platform->stopSound(sound);
}

void Java_com_sometrik_framework_MyGLSurfaceView_createSound(JNIEnv* env, jobject thiz) {
	createSound(thiz);
}

void Java_com_sometrik_framework_MyGLRenderer_onResize(JNIEnv* env, jobject thiz, float x, float y) {
	platform->onResize(x, y);
}

void Java_com_sometrik_framework_MyGLSurfaceView_settingsCreator(JNIEnv* env, jobject thiz, jobject settings, jint id) {
	jobject javaSettings = env->NewGlobalRef(settings);
	platform->settingsCreator(javaSettings, id);
}

void Java_com_sometrik_framework_MyGLSurfaceView_menuPressed(JNIEnv* env, jobject thiz) {
	platform->menuPressed();
}

void Java_com_sometrik_framework_MyGLSurfaceView_touchEvent(JNIEnv* env, jobject thiz, int mode, int fingerIndex, long time, float x, float y) {
	platform->onTouchesEvent(&thiz, mode, fingerIndex, time, x, y);
}

jboolean Java_com_sometrik_framework_MyGLSurfaceView_update(JNIEnv* env, jobject thiz) {
	if (platform->update()) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}

void Java_com_sometrik_framework_MyGLRenderer_onInit(JNIEnv* env, jobject thiz, jobject assetManager, jobject surface) {
  if (!platform.get()) {
  	float displayScale = 1.0f;
  	bool hasEs3 = false;
  	const char* glslVersion = hasEs3 ? "#version es 300" : "#version es 100";
  	platform = std::make_shared<AndroidPlatform>(env, assetManager, surface, displayScale, glslVersion, hasEs3);
  }
	platform->onInit();
}

void Java_com_sometrik_framework_MyGLRenderer_nativeOnDraw(JNIEnv* env, jobject thiz) {
	platform->onDraw();
}

void Java_com_sometrik_framework_FrameWork_okPressed(JNIEnv* env, jobject thiz, jstring text) {

	jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
	jmethodID methodRef = env->GetMethodID(cls, "printText", "(Ljava/lang/String;)V");

	env->CallVoidMethod(thiz, methodRef, text);

}
}
