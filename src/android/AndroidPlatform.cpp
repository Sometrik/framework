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

extern FWContextBase * applicationMain();

bool AndroidPlatform::onTouchesEvent(jobject * _obj, int mode, int fingerIndex, double time, float x, float y) {

  x /= getDisplayScale();
  y /= getDisplayScale();
  switch (mode) {
  case 1:
    getApplication().onTouchEvent(TouchEvent(TouchEvent::Type::ACTION_DOWN, x, y, time, fingerIndex));
    break;
  case 2:
    getApplication().onTouchEvent(TouchEvent(TouchEvent::Type::ACTION_MOVE, x, y, time, fingerIndex));
    break;
  case 3:
    getApplication().onTouchEvent(TouchEvent(TouchEvent::Type::ACTION_UP, x, y, time, fingerIndex));
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
  jobjectArray stringArray = (jobjectArray) env->NewObjectArray(optionsVector.size(), env->FindClass("java/lang/String"), 0);
  jintArray intArray = env->NewIntArray(optionsVector.size());
  jint fill[optionsVector.size()];

  //Set values to java arrays
  for (int i = 0; i < optionsVector.size(); i++) {
    const char * text = optionsVector[i].getText().c_str();
    jstring jtext = env->NewStringUTF(text);
    env->SetObjectArrayElement(stringArray, i, jtext);
    fill[i] = optionsVector[i].getId();
    env->ReleaseStringUTFChars(jtext, text);
  }
  env->SetIntArrayRegion(intArray, 0, optionsVector.size(), fill);

  //Send values to java to create the action sheet
  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetStaticMethodID(cls, "createOptionsFromJNI", "(Lcom/sometrik/framework/FrameWork;I[I[Ljava/lang/String;)V");
  env->CallStaticVoidMethod(cls, methodRef, framework, 22, intArray, stringArray);

  //Not returning anything
  return 0;
}

bool AndroidPlatform::onUpdate(double timestamp) {
  bool shouldUpdate =  getApplication().onUpdate(timestamp);
  return shouldUpdate;
}

void
AndroidPlatform::onDraw() {
  getApplication().onDraw();
}

std::string AndroidPlatform::showTextEntryDialog(const std::string & message) {
  return "";
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
  env->ReleaseStringUTFChars(title, _title);
  env->ReleaseStringUTFChars(message, _message);
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
}

std::string AndroidPlatform::getBundleFilename(const char * filename) {
  return filename;
}

std::string AndroidPlatform::getLocalFilename(const char * filename, FileType type) {

  auto env = getJNIEnv();
  jstring path;
  jstring jfilename;
  std::string result;
  switch (type) {
  case DATABASE:
  case CACHE_DATABASE:
    jfilename = env->NewStringUTF(filename);
    path = (jstring) env->CallObjectMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "getDBPath", "(Ljava/lang/String;)Ljava/lang/String;"), jfilename);
    result = env->GetStringUTFChars(path, JNI_FALSE);
    env->ReleaseStringUTFChars(jfilename, filename);
    env->ReleaseStringUTFChars(path, result.c_str());
    return result;
  case NORMAL:
    return "";
  }
  return "";
}

std::string AndroidPlatform::loadValue(const std::string & key) {
  auto env = getJNIEnv();
  jstring jkey = env->NewStringUTF(key.c_str());
  jstring value = (jstring) env->CallObjectMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "getFromPrefs", "(Ljava/lang/String;)Ljava/lang/String;"), jkey);
  std::string result = env->GetStringUTFChars(value, JNI_FALSE);
  env->ReleaseStringUTFChars(jkey, key.c_str());
  env->ReleaseStringUTFChars(value, result.c_str());

  return result;
}

void AndroidPlatform::storeValue(const std::string & key, const std::string & value) {
  auto env = getJNIEnv();
  jstring jkey = env->NewStringUTF(key.c_str());
  jstring jvalue = env->NewStringUTF(value.c_str());
  env->CallVoidMethod(framework, env->GetMethodID(env->GetObjectClass(framework), "addToPrefs", "(Ljava/lang/String;Ljava/lang/String;)V"), jkey, jvalue);
  env->ReleaseStringUTFChars(jkey, key.c_str());
  env->ReleaseStringUTFChars(jvalue, value.c_str());
}

void AndroidPlatform::messagePoster(int message, const std::string text) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/FrameWork;ILjava/lang/String;)V");

  jstring jtext = env->NewStringUTF(text.c_str());
  env->CallStaticVoidMethod(cls, methodRef, framework, message, jtext);
  env->ReleaseStringUTFChars(jtext, text.c_str());

}

void AndroidPlatform::messagePoster(int message, int content) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/FrameWork;II)V");

  env->CallStaticVoidMethod(cls, methodRef, framework, message, content);

}

void AndroidPlatform::messagePoster(int message, const std::string title, const std::string text) {

  auto env = getJNIEnv();
  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetStaticMethodID(cls, "LeaveMessageToSurface", "(Lcom/sometrik/framework/FrameWork;ILjava/lang/String;Ljava/lang/String;)V");

  jstring jtitle = env->NewStringUTF(title.c_str());
  jstring jtext = env->NewStringUTF(text.c_str());
  env->CallStaticVoidMethod(cls, methodRef, framework, message, jtitle, jtext);
  env->ReleaseStringUTFChars(jtitle, title.c_str());
  env->ReleaseStringUTFChars(jtext, text.c_str());
}

void AndroidPlatform::postNotification(const std::string & title, const std::string & message) {
  messagePoster(4, title, message);
}

double AndroidPlatform::getTime() const {

  auto env = getJNIEnv();
  jclass systemClass = env->FindClass("Ljava/lang/System");
  double currentTime = (double)env->CallStaticLongMethod(systemClass, env->GetStaticMethodID(systemClass, "currentTimeMillis", "()L"));
  env->DeleteLocalRef(systemClass);

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


void AndroidPlatform::launchBrowser(const std::string & input_url) {
  messagePoster(3, "http://stackoverflow.com/questions/4121450/activitynotfoundexception");
}

std::shared_ptr<AndroidPlatform> platform;

extern "C" {

void Java_com_sometrik_framework_MyGLRenderer_onResize(JNIEnv* env, jobject thiz, float x, float y) {
  platform->setDisplayWidth(x);
  platform->setDisplayHeight(y);
  platform->onResize(x, y);
}

void Java_com_sometrik_framework_FrameWork_menuPressed(JNIEnv* env, jobject thiz) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "menu pressed: env = %p", env);
  platform->menuPressed();
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int mode, int fingerIndex, long time, float x, float y) {
  platform->onTouchesEvent(&thiz, mode, fingerIndex, time, x, y);
}

jboolean Java_com_sometrik_framework_MyGLRenderer_onUpdate(JNIEnv* env, jobject thiz, double timestamp) {
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
  FWContextBase * application = applicationMain();
  platform->setApplication(application);
  platform->onInit(env, gJavaVM);
  application->initialize(this);
  platform->setDisplayWidth(screenWidth);
  platform->setDisplayHeight(screenHeight);
  platform->onResize(screenWidth, screenHeight);
  application->initializeContent();
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
