#include "BombDefender.h"

#include <string.h>
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
#ifdef __ANDROID__
      __android_log_print(ANDROID_LOG_INFO, "Sometrik", "OPEN_GL ERROR: %d", err);
#else
      cerr << "OpenGL error state couldn't be reset.\n";
#endif
      return;
    }

    errLast = err;

    cerr << "got OpenGL error " << err << "\n";
  }
}

bool BombDefender::Init() {
  //    __android_log_print(ANDROID_LOG_VERBOSE, "Sometrik", "Texture is null");
  auto contextF = getPlatform().createContextFactory();
  auto context = contextF->createContext(256, 256, canvas::InternalFormat::RGBA8, true);

  context->globalAlpha = 1.0f;
  context->font.size = 50;
  context->textBaseline = "top";
  auto yoSurface = context->createSurface("picture.jpg");
  context->drawImage(*yoSurface, 0, 0, 256, 256);

  auto texture = canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  sprite.setTexture(texture);

  renderer.initialize(getPlatform());
}

void BombDefender::onDraw() {
  glm::mat4 projMat = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1000.0f, +1000.0f);
  glm::mat4 mat(1.0f);
  renderer.drawSprite(sprite, projMat, mat);
  
  checkGLError();
}

void BombDefender::onShutdown() { }

std::shared_ptr<BombDefender> application;

void applicationMain(FWPlatformBase * platform) {
  application = std::make_shared<BombDefender>(platform);
  platform->setApplication(application.get());
}
