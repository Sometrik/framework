#include "Example1.h"

#include <FrameView.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>
#include <ScrollLayout.h>
#include <Switch.h>
#include <TextField.h>
#include <ImageElement.h>

#include <iostream>

using namespace std;

Example1::Example1() : FWApplication("com.sometrik.example1")
{
  auto view = std::make_shared<FrameView>();
  view->style("background-color", "#cccccc");
  addChild(view);
  
  auto layout = std::make_shared<LinearLayout>(FW_VERTICAL);
  view->addChild(layout);

  auto image = std::make_shared<ImageElement>("test.png");
  layout->addChild(image);

  auto title = make_shared<TextLabel>("Hello again!", 1234);
  title->style("font-size", 20);
  title->style("background-color", "#e03030");
  layout->addChild(title);

  auto nameLayout = make_shared<LinearLayout>(FW_HORIZONTAL);
  layout->addChild(nameLayout);
  nameLayout->addChild(make_shared<TextLabel>("Kirjoita nimesi:"));
  
  textField = make_shared<TextField>();
  nameLayout->addChild(textField);
        
  layout->addChild(make_shared<Switch>("On", "Off"));

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

void
Example1::onCommandEvent(CommandEvent & ev) {
  getLogger().println("got command event " + textField->getValue());
  
  getChildById(1234).text("Hello " + textField->getValue() + "!");
}


FWApplication * applicationMain() {
  return new Example1();  
}
