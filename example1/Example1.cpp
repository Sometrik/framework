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
Example1::Init() {

  auto contextF = getPlatform().createContextFactory();
	auto context = contextF->createContext(800, 800, canvas::InternalFormat::RGBA8, true);


		context->globalAlpha = 1.0f;
		context->font.size = 50;
		context->textBaseline = "top";
	context->strokeText("Olen Mikko osaan lukea ja kirjoittaa", 20, 100);

	auto yoSurface = context->createSurface("picture.jpg");
	context->drawImage(*yoSurface, 120, 120, 400, 400);

	dynamic_cast<AndroidPlatform&>(getPlatform()).showCanvas(dynamic_cast<canvas::ContextAndroid&>(*context));
}

void
Example1::onDraw() {
}

void
Example1::onShutdown() {
}

std::shared_ptr<Example1> application;

void applicationMain(FWPlatformBase * platform) {
	application = std::make_shared<Example1>(platform);
	platform->setApplication(application.get());
}
