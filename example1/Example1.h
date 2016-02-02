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

class Example1 : public FWContextBase {
public:
 Example1(AndroidPlatform * _platform) : FWContextBase(_platform) { }

  bool Init() { }
  void onDraw() { }
  void onShutdown() { }
  
 private:
};
