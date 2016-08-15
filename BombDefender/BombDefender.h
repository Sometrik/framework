/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <FWContextBase.h>
#include <FWPlatformBase.h>

class BombDefender : public FWContextBase {
public:
	Example1(FWPlatformBase * _platform) : FWContextBase(_platform) { }

  bool Init();
  void onDraw();
  void onShutdown();  
};
