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

#include <pthread.h>

#include <TouchEvent.h>
#include <TextEvent.h>
#include <CommandEvent.h>
#include <DrawEvent.h>
#include <UpdateEvent.h>
#include <ResizeEvent.h>


#include <android_fopen.h>

using namespace std;

extern FWApplication * applicationMain();

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

void
AndroidPlatform::setOpenGLView(jobject surface){
  JNIEnv * env = getJNIEnv();
  window = ANativeWindow_fromSurface(env, surface);
//  application->initializeContent();
//  pthread_create(&_threadId, 0, threadStartCallback, this);
}
bool
AndroidPlatform::initializeRenderer(){

  const EGLint attribs[] = {
         EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
         EGL_BLUE_SIZE, 8,
         EGL_GREEN_SIZE, 8,
         EGL_RED_SIZE, 8,
         EGL_NONE
     };
     EGLDisplay _display;
     EGLConfig _config;
     EGLint numConfigs;
     EGLint format;
     EGLSurface _surface;
     EGLContext _context;
     EGLint width;
     EGLint height;
     GLfloat ratio;

     if ((_display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
//         LOG_ERROR("eglGetDisplay() returned error %d", eglGetError());
         return false;
     }
     if (!eglInitialize(_display, 0, 0)) {
//         LOG_ERROR("eglInitialize() returned error %d", eglGetError());
         return false;
     }

     if (!eglChooseConfig(_display, attribs, &_config, 1, &numConfigs)) {
//         LOG_ERROR("eglChooseConfig() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     if (!eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format)) {
//         LOG_ERROR("eglGetConfigAttrib() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     ANativeWindow_setBuffersGeometry(window, 0, 0, format);

     if (!(_surface = eglCreateWindowSurface(_display, _config, window, 0))) {
//         LOG_ERROR("eglCreateWindowSurface() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     if (!(_context = eglCreateContext(_display, _config, 0, 0))) {
//         LOG_ERROR("eglCreateContext() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     if (!eglMakeCurrent(_display, _surface, _surface, _context)) {
//         LOG_ERROR("eglMakeCurrent() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     if (!eglQuerySurface(_display, _surface, EGL_WIDTH, &width) ||
         !eglQuerySurface(_display, _surface, EGL_HEIGHT, &height)) {
//         LOG_ERROR("eglQuerySurface() returned error %d", eglGetError());
//         destroy();
         return false;
     }

     display = _display;
     surface = _surface;
     context = _context;

     glDisable(GL_DITHER);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
     glClearColor(0, 0, 0, 0);
     glEnable(GL_CULL_FACE);
//     glShadeModel(GL_SMOOTH);
     glEnable(GL_DEPTH_TEST);

     glViewport(0, 0, width, height);

     ratio = (GLfloat) width / height;
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glFrustumf(-ratio, ratio, -1, 1, 1, 10);
     getLogger().println("Piiiip");
     return true;
}
void
AndroidPlatform::startThread(){
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 1");
  pthread_create(&_threadId, 0, threadStartCallback, this);
}
void
AndroidPlatform::stopThread(){
//  pthread_join(_threadId, 0);
}

void
AndroidPlatform::drawFrame()
{
  glClearColor(0, 18, 200, 0);
}

void
AndroidPlatform::renderLoop() {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Looping louie");
  bool renderingEnabled = true;

  while (renderingEnabled) {

    pthread_mutex_lock (&_mutex);

    // process incoming messages
//    switch (_msg) {
//
//    case MSG_WINDOW_SET:
//      initialize();
//      break;
//
//    case MSG_RENDER_LOOP_EXIT:
//      renderingEnabled = false;
//      destroy();
//      break;
//
//    default:
//      break;
//    }
//    _msg = MSG_NONE;

    if (display) {
      drawFrame();
      getLogger().println("drawing");
      if (!eglSwapBuffers(display, surface)) {
        getLogger().println("error eglSwapBuffers");
      }
    }

    pthread_mutex_unlock(&_mutex);
  }

  getLogger().println("Looping Louie is out");
}
void* AndroidPlatform::threadStartCallback(void *myself) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "About to start thread 2");
  AndroidPlatform * aplatform = (AndroidPlatform*) myself;

  aplatform->renderLoop();
  pthread_exit(0);

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

  ResizeEvent ev(platform->getTime(), width / getDisplayScale(), height / getDisplayScale(), width, height);
  platform->postEvent(platform->getActiveViewId(), ev);
}

void Java_com_sometrik_framework_FrameWork_menuPressed(JNIEnv* env, jobject thiz) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "menu pressed: env = %p", env);

  CommandEvent ce(platform->getTime(), FW_ID_MENU);
  platform->postEvent(platform->getActiveViewId(), ce);
}

void Java_com_sometrik_framework_FrameWork_touchEvent(JNIEnv* env, jobject thiz, int mode, int fingerIndex, long time, float x, float y) {
  x /= platform->getDisplayScale();
  y /= platform->getDisplayScale();
  switch (mode) {
  case 1:
    {
      TouchEvent ev(TouchEvent::ACTION_DOWN, x, y, time / 1000.0, fingerIndex);
      platform->postEvent(platform->getActiveViewId(), ev);
    }
    break;
  case 2:
    {
      TouchEvent ev(TouchEvent::ACTION_MOVE, x, y, time / 1000.0, fingerIndex);
      platform->postEvent(platform->getActiveViewId(), ev);
    }
    break;
  case 3:
    {
      TouchEvent ev(TouchEvent::ACTION_UP, x, y, time / 1000.0, fingerIndex);
      platform->postEvent(platform->getActiveViewId(), ev);
    }
    break;
  }
}

jboolean Java_com_sometrik_framework_MyGLRenderer_onUpdate(JNIEnv* env, jobject thiz, double timestamp) {
  UpdateEvent ev(platform->getTime());
  platform->postEvent(platform->getActiveViewId(), ev);
  return ev.isRedrawNeeded();
}

static JavaVM * gJavaVM = 0;
void Java_com_sometrik_framework_FrameWork_onInit(JNIEnv* env, jobject thiz, jobject assetManager, float screenWidth, float screenHeight, float displayScale, bool hasEs3) {
  if (!platform.get()) {
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Creating Platform");
    const char* glslVersion = "#version 100"; // "#version es 300"

    AAssetManager* manager = AAssetManager_fromJava(env, assetManager);
    android_fopen_set_asset_manager(manager);

    platform = std::make_shared<AndroidPlatform>(env, assetManager, thiz, displayScale, glslVersion, hasEs3);
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
//  platform->onResize(screenWidth, screenHeight);
  application->initializeContent();
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Init end");
}

void Java_com_sometrik_framework_FrameWork_nativeSetSurface(JNIEnv* env, jobject thiz, jobject surface){
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "going for it");
  platform->setOpenGLView(surface);
  platform->initializeRenderer();
  platform->startThread();
}

void Java_com_sometrik_framework_NativeLooper_test(JNIEnv* env, jobject thiz) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Test");
}

void Java_com_sometrik_framework_MyGLRenderer_nativeOnDraw(JNIEnv* env, jobject thiz) {
  DrawEvent ev(platform->getTime());
  platform->postEvent(platform->getActiveViewId(), ev);
  platform->clearRedrawNeeded();
}

void Java_com_sometrik_framework_FrameWork_okPressed(JNIEnv* env, jobject thiz, jstring text) {

  jclass cls = env->FindClass("com/sometrik/framework/FrameWork");
  jmethodID methodRef = env->GetMethodID(cls, "printText", "(Ljava/lang/String;)V");

  env->CallVoidMethod(thiz, methodRef, text);

}

void Java_com_sometrik_framework_FrameWork_buttonClicked(JNIEnv* env, jobject thiz, jint id) {
  CommandEvent ev(platform->getTime(), 0);
  platform->postEvent(id, ev);
}
void Java_com_sometrik_framework_FrameWork_textChangedEvent(JNIEnv* env, jobject thiz, jint id, jstring jtext) {
  const char * text = env->GetStringUTFChars(jtext, 0);
  TextEvent ev(platform->getTime(), text);
  env->ReleaseStringUTFChars(jtext, text);
  platform->postEvent(id, ev);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "JNI_Onload on AndroidPlatform");
  gJavaVM = vm;
  if (gJavaVM == NULL){
    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "problem on load. JavaVm is null");

  }

  return JNI_VERSION_1_6;
}
}
