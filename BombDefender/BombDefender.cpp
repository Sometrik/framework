#include "Example1.h"

#include <string.h>
#include <GLES3/gl3.h>
#include <jni.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <shader_program.h>
#include <Context.h>
#include <AndroidPlatform.h>
#include <ContextAndroid.h>

using namespace std;

bool
BombDefender::Init() {

}

void
BombDefender::onDraw() {
}

void
BombDefender::onShutdown() {
}

std::shared_ptr<Example1> application;

void applicationMain(FWPlatformBase * platform) {
	application = std::make_shared<Example1>(platform);
	platform->setApplication(application.get());
}
