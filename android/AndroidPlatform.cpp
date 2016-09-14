/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <string.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include "Menu.h"
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
#include <FWContextBase.h>

#include <android_fopen.h>

using namespace gpufw;
using namespace std;

extern void applicationMain(FWPlatformBase * platform);

bool AndroidPlatform::onTouchesEvent(jobject * _obj, int mode, int fingerIndex, double time, float x, float y) {

  x /= getDisplayScale();
  y /= getDisplayScale();
  switch (mode) {
  case 1:
    getApplication().touchesBegin(x, y, time, fingerIndex);
    break;
  case 2:
    getApplication().touchesEnded(x, y, time, fingerIndex);
    break;
  case 3:
    getApplication().touchesMoved(x, y, time, fingerIndex);
    break;
  }

  return true;
}

void AndroidPlatform::onResize(int width, int height) {

  __android_log_print(ANDROID_LOG_ERROR, "Sometrik", "resize: %d %d ", width, height);
  getApplication().onResize(width / getDisplayScale(), height / getDisplayScale(), width, height);
}

void AndroidPlatform::menuPressed() {

  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Platform menupressed called");

//	jclass handlerClass = env->GetObjectClass(handler);
//	jmethodID emptyMessageMethod = env->GetMethodID(handlerClass, "sendEmptyMessage", "(I)Z");
//	env->CallVoidMethod(handler, emptyMessageMethod, 1);

  //--------- creating actionSheet for debug
//	FWActionSheet debugSheet = FWActionSheet("This is title");
//	debugSheet.addOption(1, "Social Media");
//	debugSheet.addOption(2, "Application Settings");
//	debugSheet.addOption(3, "Login Settings");
  //------------------------

//		showActionSheet(FWRect(), debugSheet);
}

int AndroidPlatform::showActionSheet(const FWRect & rect, const FWActionSheet & sheet) {

  //Initialize java int and string arrays
  auto env = getJNIEnv();
  std::vector<FWOption> optionsVector = sheet.getOptions();
  jobjectArray stringArray = (jobjectArray) env->NewObjectArray(optionsVector.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
  jintArray intArray = env->NewIntArray(optionsVector.size());
  jint fill[optionsVector.size()];

  //Set values to java arrays
  for (int i = 0; i < optionsVector.size(); i++) {
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

bool AndroidPlatform::onUpdate(double timestamp) {
  getApplication().onUpdate(timestamp);
  return false;
}

void
AndroidPlatform::onDraw() {
  getApplication().onDraw();
}

std::string AndroidPlatform::showTextEntryDialog(const std::string & message) {

}

void AndroidPlatform::showMessageBox(const std::string & title, const std::string & message) {

  messagePoster(5, title, message);
}

void AndroidPlatform::createInputDialog(const char * _title, const char * _message, int params) {

  auto env = getJNIEnv();
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

void AndroidPlatform::showCanvas(canvas::ContextAndroid & context) {

  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "showCanvas called");
  auto env = getJNIEnv();
  jclass cls = env->GetObjectClass(framework);
  jmethodID methodRef = env->GetMethodID(cls, "setNativeCanvas", "(Landroid/graphics/Bitmap;)V");
  env->CallVoidMethod(framework, methodRef, dynamic_cast<canvas::AndroidSurface&>(context.getDefaultSurface()).getBitmap());

}

void
AndroidPlatform::onInit(JNIEnv * env, JavaVM * _gJavaVM) {
//  gJavaVM = NULL;
//  env->GetJavaVM(&gJavaVM);
  gJavaVM = _gJavaVM;
  gJavaVM->AttachCurrentThread(&env, NULL);
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "checking gjavavm");
  if (gJavaVM == NULL){
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "1");
  }
  if (_gJavaVM == NULL){
      __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "2");
    }
  getApplication().Init();

}

void AndroidPlatform::createOptions() {

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

std::string AndroidPlatform::getBundleFilename(const char * filename) {

  auto env = getJNIEnv();
  jstring path = (jstring) env->CallObjectMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "getResourcePath", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(filename));
  std::string result = env->GetStringUTFChars(path, JNI_FALSE);

  return result;
}

std::string AndroidPlatform::getLocalFilename(const char * filename, FileType type) {

  auto env = getJNIEnv();
  jstring path;
  std::string result;
  switch (type) {
  case DATABASE:
  case CACHE_DATABASE:
    path = (jstring) env->CallObjectMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "getDBPath", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(filename));
    result = env->GetStringUTFChars(path, JNI_FALSE);
    return result;
  case NORMAL:
    return "";
  }
}

std::string AndroidPlatform::loadValue(const std::string & key) {
  auto env = getJNIEnv();
  jstring value = (jstring) env->CallObjectMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "getFromPrefs", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(key.c_str()));
  std::string result = env->GetStringUTFChars(value, JNI_FALSE);

  return result;
}

void AndroidPlatform::storeValue(const std::string & key, const std::string & value) {
  auto env = getJNIEnv();
  env->CallVoidMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V"), env->NewStringUTF(key.c_str()), env->NewStringUTF(value.c_str()));
}

void AndroidPlatform::messagePoster(int message, const std::string text) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
  jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/MyGLSurfaceView;ILjava/lang/String;)V");

  env->CallStaticVoidMethod(cls, methodRef, framework, message, env->NewStringUTF(text.c_str()));
}

void AndroidPlatform::messagePoster(int message, const std::string title, const std::string text) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
  jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/MyGLSurfaceView;ILjava/lang/String;Ljava/lang/String;)V");

  env->CallStaticVoidMethod(cls, methodRef, framework, message, env->NewStringUTF(title.c_str()), env->NewStringUTF(text.c_str()));
}

void AndroidPlatform::setupLooper() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "setupLooper called");

  auto env = getJNIEnv();
  jclass nativeLooperClass = env->FindClass("com/sometrik/framework/NativeLooper");
  jmethodID createLooper = env->GetMethodID(nativeLooperClass, "<init>", "()V");

  jobject nativeLooper = env->NewObject(nativeLooperClass, createLooper);

  jclass looperClass = env->FindClass("android/os/Looper");
  jmethodID looperPrepareMethod = env->GetStaticMethodID(looperClass, "prepare", "()V");
  jmethodID runMethod = env->GetMethodID(nativeLooperClass, "run", "()V");
  jmethodID printMethod = env->GetMethodID(nativeLooperClass, "print", "()V");
  jmethodID loopMethod = env->GetMethodID(nativeLooperClass, "loop", "()V");
  jmethodID getHandlerMethod = env->GetMethodID(nativeLooperClass, "getHandler", "()Landroid/os/Handler;");

  env->CallVoidMethod(nativeLooper, printMethod);
  env->CallVoidMethod(nativeLooper, runMethod);
  handler = env->CallObjectMethod(nativeLooper, getHandlerMethod);
  env->CallVoidMethod(nativeLooper, loopMethod);
  env->CallVoidMethod(nativeLooper, printMethod);

}

void AndroidPlatform::settingsCreator(jobject settings, jint menuId) {

  //Tähän jonkinlainen switchi id:n mukaan, minkälainen preferenssivalikko tehdään.
  // preferenssi on tällä hetkellä mode, id, nimi ja niitä voi luoda settingsin listaan
  //kutsumalla createMenuItem. Tällä hetkellä sitä kutsutaan, joka kerta kun asettaa uuden

  auto env = getJNIEnv();
  jclass checkCls = env->FindClass("com/sometrik/framework/MyGLSurfaceView");
  jmethodID methodReff = env->GetMethodID(checkCls, "checkSettings", "(Lcom/sometrik/framework/Settings;)V");

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

void AndroidPlatform::postNotification(const std::string & title, const std::string & message) {
  messagePoster(4, title, message);
}

double AndroidPlatform::getTime() const {

  auto env = getJNIEnv();
  jclass frameClass = env->GetObjectClass(framework);
  double currentTime = env->CallStaticDoubleMethod(frameClass, env->GetStaticMethodID(frameClass, "getTime", "()D"));

  return currentTime;
}

JNIEnv* AndroidPlatform::getJNIEnv() const {
  if (gJavaVM == NULL) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JavaVM is null");
    return NULL;
  }

  JNIEnv *Myenv = NULL;
  gJavaVM->GetEnv((void**)&Myenv, JNI_VERSION_1_6);
  if (Myenv == NULL){
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Env is null");
  }
  return Myenv;
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

void AndroidPlatform::playSound(jobject sound) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/Media");
  jmethodID methodRef = env->GetMethodID(cls, "play", "()V");

  env->CallVoidMethod(sound, methodRef);

}

void AndroidPlatform::stopSound(jobject sound) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/Media");
  jmethodID methodRef = env->GetMethodID(cls, "stop", "()V");

  env->CallVoidMethod(sound, methodRef);

}

void AndroidPlatform::launchBrowser(const std::string & input_url) {
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
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "menu pressed: env = %p", env);
  platform->menuPressed();
}

void Java_com_sometrik_framework_MyGLSurfaceView_touchEvent(JNIEnv* env, jobject thiz, int mode, int fingerIndex, long time, float x, float y) {
  platform->onTouchesEvent(&thiz, mode, fingerIndex, time, x, y);
}

jboolean Java_com_sometrik_framework_MyGLSurfaceView_onUpdate(JNIEnv* env, jobject thiz, double timestamp) {
  if (platform->onUpdate(timestamp)) {
    return JNI_TRUE;
  } else {
    return JNI_FALSE;
  }
}

static JavaVM * gJavaVM = 0;
void Java_com_sometrik_framework_MyGLRenderer_onInit(JNIEnv* env, jobject thiz, jobject assetManager, jobject surface, float screenWidth, float screenHeight, float displayScale, bool hasEs3) {
  if (!platform.get()) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");
    const char* glslVersion = "#version 100"; // "#version es 300"

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    platform = std::make_shared<AndroidPlatform>(env, assetManager, surface, displayScale, glslVersion, hasEs3);
  }
  applicationMain(platform.get());
  platform->onResize(screenWidth, screenHeight);
  platform->onInit(env, gJavaVM);
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
}

void Java_com_sometrik_framework_NativeLooper_test(JNIEnv* env, jobject thiz) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Test");
//  platform->onInit();
}

void Java_com_sometrik_framework_MyGLRenderer_nativeOnDraw(JNIEnv* env, jobject thiz) {
  platform->onDraw();
}

void Java_com_sometrik_framework_FrameWork_okPressed(JNIEnv* env, jobject thiz, jstring text) {

  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetMethodID(cls, "printText", "(Ljava/lang/String;)V");

  env->CallVoidMethod(thiz, methodRef, text);

}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JNI_Onload on AndroidPlatform");
  gJavaVM = vm;

  return JNI_VERSION_1_6;
}
}
