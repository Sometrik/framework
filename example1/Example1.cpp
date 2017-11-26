#include "Example1.h"

#include <FrameView.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Button.h>
#include <ScrollLayout.h>
#include <Switch.h>
#include <TextField.h>
#include <ImageElement.h>
#include <NavigationBar.h>
#include <NavigationBarItem.h>
#include <NavigationDrawer.h>
#include <FrameLayout.h>
#include <ProgressBar.h>
#include <FlipperLayout.h>
#include <ActionBar.h>
#include <TimerEvent.h>

#include "ImageDialog.h"

#include <iostream>

#define ID_CLICK_ME_BUTTON 1
#define ID_SHOW_MENU_BUTTON 2

using namespace std;

Example1::Example1() : FWApplication("com.sometrik.example1")
{
  createTimer(1000);
  
  actionBar = std::make_shared<ActionBar>();
  addChild(actionBar);
  
  navigationDrawer = std::make_shared<NavigationDrawer>();
  navigationDrawer->style("background", "#cccccc");
  navigationDrawer->addChild(make_shared<TextLabel>("Hello sidebar!"));
  navigationDrawer->addChild(make_shared<Button>("OK"));
  addChild(navigationDrawer);
  
  auto view = std::make_shared<FrameView>();
  view->style("background-color", "#555555");
  addChild(view);
  
  auto flipper = std::make_shared<FlipperLayout>();
  view->addChild(flipper);
  
  auto firstPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  flipper->addChild(firstPage);
 
  auto secondPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  flipper->addChild(secondPage);
  
  auto image = std::make_shared<ImageElement>("test.png");
  firstPage->addChild(image);
  
  auto title = make_shared<TextLabel>("Hello again!", 1234);
  title->style("font-size", 20);
  title->style("background-color", "#e03030");
  title->style("shadow", 5);
  title->style("border", "#801010");
  title->style("border-radius", 5);
  firstPage->addChild(title);

  auto nameLayout = make_shared<LinearLayout>(FW_HORIZONTAL);
  firstPage->addChild(nameLayout);
  nameLayout->addChild(make_shared<TextLabel>("Kirjoita nimesi:"));
  
  textField = make_shared<TextField>();
  nameLayout->addChild(textField);
  
  auto buttonLayout = std::make_shared<LinearLayout>(FW_HORIZONTAL);
  buttonLayout->addChild(make_shared<Button>("Click me!", ID_CLICK_ME_BUTTON)).style("background", "#30e030").style("border-radius", 5);
  buttonLayout->addChild(make_shared<Button>("Show menu", ID_SHOW_MENU_BUTTON));
  buttonLayout->addChild(make_shared<Button>("Useless button"));
  firstPage->addChild(buttonLayout);

  auto scrollLayout = std::make_shared<ScrollLayout>();
  scrollLayout->style("min-height", 200);
  firstPage->addChild(scrollLayout);
  auto scrollContent = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout->addChild(scrollContent);
  
  for (int i = 0; i < 100; i++) {
    scrollContent->addChild(std::make_shared<TextLabel>("Number: " + to_string(i)));
  }
  
  firstPage->addChild(make_shared<Switch>("On", "Off"));

  auto navigationBar = std::make_shared<NavigationBar>();
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 1"));
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 2"));
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 3"));

  view->addChild(navigationBar);
  
  secondPage->addChild(make_shared<ProgressBar>());
}

void
Example1::onCommandEvent(CommandEvent & ev) {
  cerr << "got command event: " << ev.getElementId() << endl;
  switch (ev.getElementId()) {
    case ID_CLICK_ME_BUTTON: {
      cerr << "showing dialog" << endl;
      auto dialog = make_shared<ImageDialog>();
      dialog->showModal(this);
    }
      break;
    case ID_SHOW_MENU_BUTTON: {
      cerr << "showing menu" << endl;
      navigationDrawer->show();
    }
      break;
  }
}

void
Example1::onTimerEvent(TimerEvent & ev) {
  cerr << "timer" << endl;
}

FWApplication * applicationMain() {
  return new Example1();  
}
