#include "BombDefender.h"

#include <string>
#include <cstdlib>
#include <GLES3/gl3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <Context.h>
#include <VBO.h>
#include <OpenGLTexture.h>
#include <TextureRef.h>

using namespace std;
using namespace gpufw;

static void checkGLError() {
  GLenum errLast = GL_NO_ERROR;
  for (;;) {
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
      return;
    }

    // normally the error is reset by the call to glGetError() but if
    // glGetError() itself returns an error, we risk looping forever here
    // so check that we get a different error than the last time
    if (err == errLast) {
      cerr << "OpenGL error state couldn't be reset.\n";
      return;
    }

    errLast = err;

    cerr << "got OpenGL error " << err << "\n";
  }
}

bool BombDefender::Init() {
  renderer.initialize(getPlatform());

  //    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Texture is null");
  auto contextF = getPlatform().createContextFactory();
  auto context = contextF->createContext(256, 256, canvas::InternalFormat::RGBA8, true);

  context->globalAlpha = 1.0f;
  context->font.size = 50;
  context->textBaseline = "top";
  auto yoSurface = context->createSurface("picture.jpg");
  context->drawImage(*yoSurface, 0, 0, 256, 256);

  auto texture = canvas::OpenGLTexture::createTexture(context->getDefaultSurface());

  for (unsigned int i = 0; i < 1000; i++) {
    auto sprite = std::make_shared<GameObject>();
    sprite->setPosition(glm::vec2((float)rand() / RAND_MAX * getLogicalWidth(),
				  (float)rand() / RAND_MAX * getLogicalHeight()
				  ));
    sprite->setVelocity(glm::vec2((float)rand() / RAND_MAX * 10.0f - 5,
				  (float)rand() / RAND_MAX * 10.0f - 5
				  ));
    sprite->setWidth(64);
    sprite->setHeight(64);
    sprite->setTexture(texture);
    sprites.push_back(sprite);
  }
}

void BombDefender::onDraw() {
  glm::mat4 projMat = glm::ortho(0.0f, (float)getLogicalWidth(), 0.0f, (float)getLogicalHeight(), -1000.0f, +1000.0f);
  glm::mat4 mat(1.0f);

  for (auto & sprite : sprites) {
    renderer.drawSprite(*sprite, projMat, mat);
  }
  
  checkGLError();
}

bool
BombDefender::onUpdate(double timestamp) {
  float dt = prev_update_time > 0 ? timestamp - prev_update_time : 0;
  prev_update_time = timestamp;

  for (auto & sprite : sprites) {
    sprite->update(dt);
  }

  return true;
}

void BombDefender::onShutdown() { }

std::shared_ptr<BombDefender> application;

void applicationMain(FWPlatformBase * platform) {
  application = std::make_shared<BombDefender>(platform);
  platform->setApplication(application.get());
}
