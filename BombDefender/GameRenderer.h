#include <PrimitiveRenderer.h>

#include <shader_program.h>

class FWPlatformBase;

class GameRenderer : public PrimitiveRenderer {
 public:
  GameRenderer() { }

  void initialize(FWPlatformBase & platform);
  void drawSprite(const Sprite & sprite, const glm::mat4 & projMat, const glm::mat4 & mvMat);

 private:
  std::shared_ptr<gpufw::shader_program> sprite_program;
  bool is_initialized = false;
};
