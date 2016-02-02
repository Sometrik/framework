/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <string.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <jni.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <android/bitmap.h>
#include <shader_program.h>
#include <AndroidPlatform.h>
#include <FWContextBase.h>

class Example1 : FWContextBase {

public:
	Example1(JNIEnv * _env, jobject _mgr, jobject _framework, float _display_scale, const char * _glsl_version, bool _has_es3) :
		FWContextBase(AndroidPlatform(_env, _mgr, _framework, _display_scale, _glsl_version, _has_es3)){ }


   bool Init() { }
   void onDraw() { }
   void onShutdown() { }


private:
   int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;

};
