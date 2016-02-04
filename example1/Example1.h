/*
 * Copyright (C) Sometrik oy 2015
 *
 */

#include <FWContextBase.h>
#include <AndroidPlatform.h>

class Example1 : public FWContextBase {
public:
	Example1(AndroidPlatform * _platform) : FWContextBase(_platform), platform(_platform) { }

  bool Init();
  void onDraw();
  void onShutdown();
  
 private:
  AndroidPlatform * platform;
};
