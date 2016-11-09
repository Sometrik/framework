/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <string.h>
#include <android/log.h>
#include "Menu.h"
#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <iostream>
#include <android/bitmap.h>
#include <ContextAndroid.h>
#include <AndroidClient.h>
#include <AndroidPlatform.h>
#include <FWApplication.h>

#include <TouchEvent.h>
#include <CommandEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>

#include <android_fopen.h>

using namespace std;

extern FWApplication * applicationMain();

void
AndroidPlatform::onTouchesEvent(jobject * _obj, int mode, int fingerIndex, double time, float x, float y) {
  x /= getDisplayScale();
  y /= getDisplayScale();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(TouchEvent::ACTION_DOWN, x, y, time, fingerIndex);
      postEvent(getActiveViewId(), ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(TouchEvent::ACTION_MOVE, x, y, time, fingerIndex);
      postEvent(getActiveViewId(), ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(TouchEvent::ACTION_UP, x, y, time, fingerIndex);
      postEvent(getActiveViewId(), ev);
    }
    break;
  }
}

void
AndroidPlatform::onResize(int width, int height) {
  __android_log_print(ANDROID_LOG_ERROR, "Sometrik", "resize: %d %d ", width, height);
  ResizeEvent ev(getTime(), width / getDisplayScale(), height / getDisplayScale(), width, height);
  postEvent(getActiveViewId(), ev);
}

void
AndroidPlatform::menuPressed() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Platform menupressed called");
  CommandEvent ce(getTime(), FW_ID_MENU);
  postEvent(getActiveViewId(), ce);

//	jclass handlerClass = env->GetObjectClass(handler);
//	jmethodID emptyMessageMethod = env->GetMethodID(handlerClass, "sendEmptyMessage", "(I)Z");
//	env->CallVoidMethod(handler, emptyMessageMethod, 1);
}

void
AndroidPlatform::onUpdate(double timestamp) {
  UpdateEvent ev(getTime());
  postEvent(getActiveViewId(), ev);
}

void
AndroidPlatform::onDraw() {
  DrawEvent ev(getTime());
  postEvent(getActiveViewId(), ev);
}

void AndroidPlatform::showCanvas(canvas::ContextAndroid & context) {

  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "showCanvas called");
  auto env = getJNIEnv();
  jclass cls = env->GetObjectClass(framework);
  jmethodID methodRef = env->GetMethodID(cls, "setNativeCanvas", "(Landroid/graphics/Bitmap;)V");
  env->CallVoidMethod(framework, methodRef, dynamic_cast<canvas::AndroidSurface&>(context.getDefaultSurface()).getBitmap());

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


void
AndroidPlatform::sendMessage(const Message & message) {
  FWPlatform::sendMessage(message);
  
  auto env = getJNIEnv();
   jclass frameworkCls = env->FindClass("com/sometrik/framework/FrameWork");
   jclass messageCls = env->FindClass("com/sometrik/framework/NativeMessage");
   jmethodID sendMessageMethod = env->GetStaticMethodID(frameworkCls, "sendMessage", "(Lcom/sometrik/framework/FrameWork;Lcom/sometrik/framework/NativeMessage;)V");
   jmethodID messageConstructor = env->GetMethodID(messageCls, "<init>", "(IIILjava/lang/String;Ljava/lang/String;)V");

   int messageTypeId = int(message.getType());
   const char * textValue = message.getTextValue().c_str();
   const char * textValue2 = message.getTextValue2().c_str();
   jstring jtextValue = env->NewStringUTF(textValue);
   jstring jtextValue2 = env->NewStringUTF(textValue2);


   jobject jmessage = env->NewObject(messageCls, messageConstructor, messageTypeId, message.getInternalId(), message.getChildInternalId(), jtextValue, jtextValue2);
   env->CallStaticVoidMethod(frameworkCls, sendMessageMethod, framework, jmessage);
   //Fix these releases
//  env->ReleaseStringUTFChars(jtextValue, textValue);
//  env->ReleaseStringUTFChars(jtextValue2, textValue2);
}

double
AndroidPlatform::getTime() const {
  auto env = getJNIEnv();
  jclass systemClass = env->FindClass("java/lang/System");
  double currentTime = (double)env->CallStaticLongMethod(systemClass, env->GetStaticMethodID(systemClass, "currentTimeMillis", "()J"));
  env->DeleteLocalRef(systemClass);

  return currentTime;
}

void
AndroidPlatform::buttonClicked(int id) {
  CommandEvent ev(getTime(), id);
  postEvent(id, ev);
}

int
AndroidPlatform::showActionSheet(const FWRect & rect, const FWActionSheet & sheet) {

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

JNIEnv *
AndroidPlatform::getJNIEnv() const {
  if (gJavaVM == NULL) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JavaVM is null");
    return NULL;
  }

  JNIEnv * Myenv = NULL;
  gJavaVM->GetEnv((void**)&Myenv, JNI_VERSION_1_6);
  if (Myenv == NULL) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Env is null");
  }
  return Myenv;
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
  platform->onUpdate(timestamp);
  return JNI_TRUE; // FIX ME: check the return value from event
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
  
  FWApplication * application = applicationMain();
  platform->setApplication(application);

  if (gJavaVM) {
    gJavaVM->AttachCurrentThread(&env, NULL);
    platform->setJavaVM(gJavaVM);
  }

  application->initialize(platform.get());
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

void Java_com_sometrik_framework_FrameWork_buttonClicked(JNIEnv* env, jint buttonId) {
  platform->buttonClicked(buttonId);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JNI_Onload on AndroidPlatform");
  gJavaVM = vm;

  return JNI_VERSION_1_6;
}
}
