#include "Example1.h"

#include <FrameView.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>
#include <ScrollLayout.h>

#include <iostream>

using namespace std;

Example1::Example1() : FWApplication("com.sometrik.example1")
{
  auto view = std::make_shared<FrameView>();
  addChild(view);

  auto layout = std::make_shared<LinearLayout>(FW_VERTICAL);
  view->addChild(layout);

  auto title = make_shared<TextLabel>("Hello World!");
  title->style("font-size", 20);
  layout->addChild(title);

  auto buttonLayout = std::make_shared<LinearLayout>(FW_HORIZONTAL);
  buttonLayout->addChild(make_shared<Button>("Click me!"));
  buttonLayout->addChild(make_shared<Button>("Cancel"));
  buttonLayout->addChild(make_shared<Button>("Button"));
  layout->addChild(buttonLayout);

  auto scrollLayout = std::make_shared<ScrollLayout>();
  scrollLayout->style("min-height", 200);
  layout->addChild(scrollLayout);
  auto scrollContent = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout->addChild(scrollContent);
  
  for (int i = 0; i < 100; i++) {
    scrollContent->addChild(std::make_shared<TextLabel>("Number: " + to_string(i)));
  }
  
}

FWApplication * applicationMain() {
  return new Example1();  
}
