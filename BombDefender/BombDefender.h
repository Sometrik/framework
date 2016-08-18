/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <FWContextBase.h>
#include <FWPlatformBase.h>
#include <Sprite.h>
#include <shader_program.h>
#include <VBO.h>

class BombDefender : public FWContextBase {
public:
	BombDefender(FWPlatformBase * _platform) : FWContextBase(_platform) { }

  bool Init();
  void onDraw();
  void onShutdown();

 private:
  void drawSprite(const Sprite & sprite);
  void use(const gpufw::shader_program & program);
  void bind(const VBO & vbo);

  std::shared_ptr<gpufw::shader_program> test_program;
};

