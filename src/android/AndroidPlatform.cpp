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

		createOptions();

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
AndroidPlatform::onInit(jobject surface) {

		// AAssetManager* manager = AAssetManager_fromJava(env, mgr);

	showMessageBox("Morty", "Don't even trip dawg");

		canvas::AndroidContextFactory factory(env, mgr);
		auto context = factory.createContext(400, 400, canvas::InternalFormat::RGB_DXT1);
		context->globalAlpha = 1.0f;
		context->font.size = 50;
		context->textBaseline = "top";
		context->textAlign = "left";
		auto yoSurface = context->createSurface("picture.jpg");
		//context->shadowBlur = context->shadowOffsetX = context->shadowOffsetY = 5.0f;
		//context->drawImage(*yoSurface, 120, 120, 400, 400);
		context->fillText("Olen Mikko osaan lukea ja kirjoittaa", 20, 100);

		//showCanvas((dynamic_cast<canvas::AndroidSurface&>(*yoSurface)).getBitmap(), surface);
		showCanvas((dynamic_cast<canvas::AndroidSurface&>(context->getDefaultSurface())).getBitmap(), surface, env);
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
		AndroidClientFactory clientFactory(env);
		auto android = clientFactory.createClient("yo", false, false);

		HTTPRequest requ = HTTPRequest(HTTPRequest::GET, "https://www.99analytics.com/test");
		requ.setFollowLocation(false);
		Authorization autor = Authorization();
		//auto resp = android->request(requ, autor);
		//auto res = android->Get("http://i.imgur.com/0rxRc6i.jpg");
		//if (res.isSuccess()) {
			//auto surfaceee = factory.createSurface((unsigned char*)res.getContent().c_str(), res.getContent().size());
			//context->drawImage(*surfaceee, 0, 0, 300, 300);
		//}
	}

void
AndroidPlatform::createOptions() {

		jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
		jmethodID methodRef = env->GetStaticMethodID(cls, "createOptionsFromJNI", "(Lcom/sometrik/framework/MyGLSurfaceView;I[I[Ljava/lang/String;)V");

		//Making an int array
		jintArray intArray;
		intArray = env->NewIntArray(4);

		jint fill[4];
		fill[0] = 1;
		fill[1] = 2;
		fill[2] = 3;
		fill[3] = 4;
		env->SetIntArrayRegion(intArray, 0, 3, fill);

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

		//Making a string Array
		jobjectArray stringArray;

		const char *strings[4] = { "first", "second", "Social Accounts", "fourth" };
		stringArray = (jobjectArray) env->NewObjectArray(4, env->FindClass("java/lang/String"), env->NewStringUTF(""));

		for (int i = 0; i < 4; i++) {
			env->SetObjectArrayElement(stringArray, i, env->NewStringUTF(strings[i]));
		}

		env->CallStaticVoidMethod(cls, methodRef, *framework, 22, intArray, stringArray);

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
		jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/MyGLSurfaceView;ILjava/lang/String;)V");

		env->CallStaticVoidMethod(cls, methodRef, framework, message, env->NewStringUTF(text.c_str()), env->NewStringUTF(text.c_str()));
	}

void
AndroidPlatform::settingsCreator(jobject thiz, jint menuId) {

		//Tähän jonkinlainen switchi id:n mukaan, minkälainen preferenssivalikko tehdään.
		// preferenssi on tällä hetkellä mode, id, nimi ja niitä voi luoda settingsin listaan
		//kutsumalla createMenuItem. Tällä hetkellä sitä kutsutaan, joka kerta kun asettaa uuden

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

		env->CallVoidMethod(thiz, methodRef, mode, id, name);
		env->CallVoidMethod(thiz, methodRef, mode2, id2, name2);
		env->CallVoidMethod(thiz, methodRef2, media1, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media2, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media2, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);
		env->CallVoidMethod(thiz, methodRef2, media3, id2, name3);

	}

void
AndroidPlatform::postNotification(const std::string & title, const std::string & message){
	messagePoster(4, message);
 }

double
AndroidPlatform::getTime() const{
	__android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "getTime called");

	jclass frameClass = env->GetObjectClass(framework);
	double currentTime = env->CallStaticDoubleMethod(frameClass, env->GetStaticMethodID(frameClass, "getTime", "()D"));
	__android_log_print(ANDROID_LOG_INFO, "Sometrik", "dubbeli = %d", currentTime);

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
	platform->settingsCreator(settings, id);
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
  	//AndroidPlatform(env, assetManager, &thiz, displayScale, glslVersion, hasEs3)->onInit();
  }
	platform->onInit(surface);
}

void Java_com_sometrik_framework_MyGLRenderer_nativeOnDraw(JNIEnv* env, jobject thiz) {
	platform->onDraw();
}

void Java_com_sometrik_framework_FrameWork_okPressed(JNIEnv* env, jobject thiz, jstring text) {

	jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
	jmethodID methodRef = env->GetMethodID(cls, "printText", "(Ljava/lang/String;)V");

	//jstring name = (*env)->NewStringUTF(env, "Hello java");

	env->CallVoidMethod(thiz, methodRef, text);

}
}
