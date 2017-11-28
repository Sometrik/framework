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

#include "FrontView.h"
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
  auto navigationLayout = std::make_shared<LinearLayout>(FW_VERTICAL);
  navigationDrawer->addChild(navigationLayout);
  navigationDrawer->style("background", "#cccccc");
  navigationLayout->addChild(make_shared<TextLabel>("Hello sidebar!"));
  navigationLayout->addChild(make_shared<Button>("OK"));
  addChild(navigationDrawer);
  
  frontView = std::make_shared<FrontView>();
  addChild(frontView);
  
  mainView = std::make_shared<FrameView>();
  mainView->style("background-color", "#555555");
  addChild(mainView);
  
  auto flipper = std::make_shared<FlipperLayout>();
  mainView->addChild(flipper);
  
  auto firstPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  flipper->addChild(firstPage);
  
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
  buttonLayout->addChild(make_shared<Button>("Useless button")).style("icon", "button.png");
  firstPage->addChild(buttonLayout);

  auto scrollLayout = std::make_shared<ScrollLayout>();
  scrollLayout->style("min-height", 200);
  firstPage->addChild(scrollLayout);
  scrollContent = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout->addChild(scrollContent);
  
  for (int i = 0; i < 100; i++) {
    scrollContent->addChild(std::make_shared<TextLabel>("Number: " + to_string(i)));
  }
  
  firstPage->addChild(make_shared<Switch>("On", "Off"));

  auto navigationBar = std::make_shared<NavigationBar>();
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 1")).style("icon", "icon1.png");
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 2")).style("icon", "icon2.png");
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 3")).style("icon", "icon3.png");

  mainView->addChild(navigationBar);
  
#if 1
  auto secondPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  secondPage->style("background", "#e03030");
  flipper->addChild(secondPage);
  
  auto scrollLayout2 = std::make_shared<ScrollLayout>();
  secondPage->addChild(scrollLayout2);
  auto scrollContent2 = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout2->addChild(scrollContent2);
  
  for (char c = 'A'; c <= 'Z'; c++) {
    string s;
    s += c;
    scrollContent2->addChild(make_shared<TextLabel>(s));
  }
  
#endif

#if 1
  auto thirdPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  thirdPage->style("background", "#e03030");
  thirdPage->style("margin", 20);
  flipper->addChild(thirdPage);
  thirdPage->addChild(make_shared<TextLabel>("THIRD PAGE")).style("font-size", "20");
  thirdPage->addChild(make_shared<ProgressBar>());
#endif
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
    case ID_SHOW_FRONT_PAGE: {
      frontView->hide();
      mainView->show();
    }
      break;
  }
}

void
Example1::onTimerEvent(TimerEvent & ev) {
#if 1
  auto & children = scrollContent->getChildren();
  cerr << "timer: front child = " << children.front()->getInternalId() << endl;
  scrollContent->reorderChildren(*(children.back()), 0);
  children.insert(children.begin(), children.back());
  children.pop_back();
#endif
}

FWApplication * applicationMain() {
  return new Example1();  
}
