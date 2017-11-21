#include "Example1.h"

#include <FrameView.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>

#include <iostream>

using namespace std;

Example1::Example1() : FWApplication("com.sometrik.example1")
{
  auto view = std::make_shared<FrameView>();
  addChild(view);

  auto layout = std::make_shared<LinearLayout>(FW_VERTICAL);
  view->addChild(layout);
  
  layout->addChild(make_shared<TextLabel>("Hello World!"));
  layout->addChild(make_shared<Button>("Click me!"));
}

FWApplication * applicationMain() {
  return new Example1();  
}
