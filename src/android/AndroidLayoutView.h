#include <jni.h>
#include <androidLogger.h>

class AndroidLayoutView {

public:
  AndroidLayoutView(JNIEnv * _env, jobject _activityObject) {
    activityObject = _env->NewGlobalRef(_framework);
    _env->GetJavaVM(&javaVM);
  }

  ~AndroidLayoutView() {
  }

  void androidInit() {
    JNIEnv * env = getJNIEnv();
    auto logger = AndroidLogger();
    logger.println("AndroidLayout androidInit begin");
    activityClass = (jclass)env->NewGlobalRef(env->FindClass("android/app/Activity"));
    viewClass = (jclass)env->NewGlobalRef(env->FindClass("android/view/View"));
    linearLayoutClass = (jclass)env-NewGlobalRef(env-FindClass("android/widget/LinearLayout"));
    buttonClass = (jclass)env->NewGlobalRef(env-FindClass("android/widget/Button"));
    contextClass = (jclass)env->NewGlobalRef(env->FindClass("android/content/Context"));
    textViewClass = (jclass)env->NewGlobalRef(env->FindClass("android/widget/TextView"));
    editTextViewClass = (jclass)env->NewGlobalRef(env->FindClass("android/widget/EditText"));
    viewGroupClass = (jclass)env->NewGlobalRef(env->FindClass("android/view/ViewGroup"));

    addViewMethod = env->GetMethodID(viewGroupClass, "addView", "(Landroid/view/View;)V")
    setContentViewMethod = env->GetMethodID(activityClass, "setContentView", "(Landroid/view/View;)V");
    linearLayoutConstructor = env->GetMethodID(linearLayoutClass,  "<init>", "(Landroid/content/Context;)V");
    buttonConstructor = env->GetMethodID(buttonClass,  "<init>", "(Landroid/content/Context;)V");
    textViewConstructor = env->GetMethodID(textViewClass,  "<init>", "(Landroid/content/Context;)V");
    editTextViewConstructor = env->GetMethodID(editTextViewClass,  "<init>", "(Landroid/content/Context;)V");
    buttonSetTextMethod = env->GetMethodID(buttonClass, "setText", "(Ljava/lang/CharSequence;)V");
    textViewSetTextMethod = env->GetMethodID(textViewClass, "setText", "(Ljava/lang/CharSequence;)V");
    editTextViewSetTextMethod = env->GetMethodID(editTextViewClass, "setText", "(Ljava/lang/CharSequence;)V");
    editTextViewGetTextMethod = env->GetMethodID(editTextViewClass, "getText", "()Ljava/lang/String;");
    linearLayoutOrientationMethod = env->GetMethodID(linearLayoutClass, "setOrientation", "(I)V");


    mainLayout = env->NewGlobalRef(env->NewObject(linearLayoutClass, linearLayoutConstructor, activityObject));


    logger.println("AndroidLayout androidInit Done");
  }

  JNIEnv * getJNIEnv() {
    JNIEnv *Myenv = NULL;
    javaVM->GetEnv((void**) &Myenv, JNI_VERSION_1_6);
    return Myenv;
  }

  //Orientation: 1 = vertical, 0 = horizontal.
  public void createLinearLayout(int orientation){
    jobject linearLayout = env->NewObject(linearLayoutClass, linearLayoutConstructor);
  }

  public void createTextView(const char * text){
    jobject textView = env->NewObject(textViewClass, textViewConstructor);
    jstring jtext = env->NewStringUTF(text);
    env->CallVoidMethod(textView, textViewSetTextMethod, jtext);
    env->ReleaseStringUTFChars(jtext, text);
  }

  public void createButton(const char * text){
    jobject button = env->NewObject(buttonClass, buttonConstructor);
    jstring jtext = env->NewStringUTF(text);
    env->CallVoidMethod(button, buttonSetTextMethod, jtext);
    env->ReleaseStringUTFChars(jtext, text);
  }

  public void createEditTextView(const char * text){
    jobject editTextView = env->NewObject(editTextViewClass, editTextViewConstructor);
    jstring jtext = env->NewStringUTF(text);
    env->CallVoidMethod(editTextView, editTextViewSetTextMethod, jtext);
    env->ReleaseStringUTFChars(jtext, text);
  }

  private void setContentView(){
    env->CallVoidMethod(viewGroupClass, setContentViewMethod, mainLayout);
  }


private:
  JavaVM * gJavaVM;
  jobject activityObject;
  jobject handler;
  jobject mainLayout;

  jclass activityClass;
  jclass viewClass;
  jclass linearLayoutClass;
  jclass buttonClass;
  jclass contextClass;
  jclass textViewClass;
  jclass editTextViewClass;
  jclass viewGroupClass;


  jmethod setContentViewMethod;
  jmethod linearLayoutConstructor;
  jmethod buttonConstructor;
  jmethod textViewConstructor;
  jmethod editTextViewConstructor;
  jmethod buttonSetTextMethod;
  jmethod textViewSetTextMethod;
  jmethod editTextViewSetTextMethod;
  jmethod editTextViewGetTextMethod;
  jmethod linearLayoutOrientationMethod;
  jmethod addViewMethod;
};
