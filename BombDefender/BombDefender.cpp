#include "BombDefender.h"

#include <string.h>
#include <GLES3/gl3.h>
#include <jni.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <Context.h>
#include <AndroidPlatform.h>
#include <ContextAndroid.h>
#include <VBO.h>

using namespace std;
using namespace gpufw;

std::shared_ptr<canvas::Context> context;
int positionX = 120;
int positionY = 120;
std::shared_ptr<canvas::Surface> yoSurface;

bool BombDefender::Init() {

  test_program = std::shared_ptr<gpufw::shader_program>(new gpufw::shader_program);

}

void BombDefender::onDraw() {
  auto contextF = getPlatform().createContextFactory();
  context = contextF->createContext(800, 800, canvas::InternalFormat::RGBA8, true);

  context->globalAlpha = 1.0f;
  context->font.size = 50;
  context->textBaseline = "top";
  context->strokeText("T�m� on Pommipeli", 20, 100);

  yoSurface = context->createSurface("picture.jpg");
  context->drawImage(*yoSurface, positionX, positionY, 400, 400);
  positionX--;
  positionY--;
//	__android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Application ondraw");
  dynamic_cast<AndroidPlatform&>(getPlatform()).showCanvas(dynamic_cast<canvas::ContextAndroid&>(*context));

}

void BombDefender::drawSprite(const Sprite & sprite){

  glm::mat4 projMat = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1000.0f, +1000.0f);
  glm::mat4 mat(1.0f);

  use(*test_program);
  test_program->setUniform("proj_matrix", projMat);
  test_program->setUniform("mv_matrix", mat);
  test_program->setUniform("s_texture", 0);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);

  glActiveTexture(GL_TEXTURE0);
//  glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

  VBO vbo;
  vbo.quad2d(10.0f, 10.0f,
  	   500.0f, 10.0f,
  	   500.0f, 500.0f,
  	   10.0f, 500.0f);
  vbo.draw();

  glBindTexture(GL_TEXTURE_2D, 0);
}

void BombDefender::onShutdown() {
}

void
BombDefender::use(const gpufw::shader_program & program) {
  int id = program.getProgramObjectId();
  if (!id) {
    assert(0);
  }
    glUseProgram(id);
}

std::shared_ptr<BombDefender> application;

void applicationMain(FWPlatformBase * platform) {
  application = std::make_shared<BombDefender>(platform);
  platform->setApplication(application.get());

}
