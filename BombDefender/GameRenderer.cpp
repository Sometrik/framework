#include "GameRenderer.h"

void
GameRenderer::initialize(FWPlatformBase & platform) {
  if (is_initialized) return;
  is_initialized = true;

  initializeBase();

  test_program = std::shared_ptr<gpufw::shader_program>(new gpufw::shader_program);
  test_program->loadShaders(getPlatform().getGLSLVersion(), "simple_sprite_shader.glsl");
  test_program->bindAttribLocation(0, "a_texCoord");
  test_program->bindAttribLocation(1, "a_position");
  test_program->link();

  glActiveTexture(GL_TEXTURE0);
}

void
GameRenderer::drawSprite(const Sprite & sprite, const glm::mat4 & projMat, const glm::mat4 & mvMat) {
  use(*sprite_program);
  sprite_program->setUniform("proj_mv_matrix", projMat * mvMat);
  sprite_program->setUniform("s_texture", 0);
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(GL_FALSE);
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite.getTexture().getTextureId());
  
  VBO vbo;
  vbo.quad2d(10.0f, 10.0f, 500.0f, 10.0f, 500.0f, 500.0f, 10.0f, 500.0f);
  
  bind(vbo);
  
  //	__android_log_print(ANDROID_LOG_INFO, "Sometrik", "BomdDefender texture id: %d", texture->getTextureId());
  //	__android_log_print(ANDROID_LOG_INFO, "Sometrik", "BomdDefender IndexBufferId id: %d", vbo.getIndexBufferId());
  
  vbo.draw();
  
  glBindTexture(GL_TEXTURE_2D, 0);
}
