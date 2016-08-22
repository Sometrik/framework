/*
 * Copyright (C) Sometrik Oy 2015
 *
 */

#include <FWContextBase.h>
#include <FWPlatformBase.h>
#include <Sprite.h>
#include <shader_program.h>
#include <VBO.h>

#include "GameRenderer.h"

class BombDefender : public FWContextBase {
public:
  BombDefender(FWPlatformBase * _platform) : FWContextBase(_platform) { }

  bool Init();
  void onDraw();
  void onShutdown();

 private:
  GameRenderer renderer;
  Sprite sprite;
};

