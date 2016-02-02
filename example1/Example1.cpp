#include "Example1.h"

#include <string.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <jni.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <android/bitmap.h>
#include <shader_program.h>
#include <AndroidPlatform.h>

using namespace std;

Example1::Example1(AndroidPlatform * _platform)
  : FWContextBase(_platform) {
}

bool
Example1::Init() {

}

void
Example1::onDraw() {
}

void
Example1::onShutdown() {
  
}
