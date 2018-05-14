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
#include <ProgressSpinner.h>
#include <FlipperLayout.h>
#include <ActionBar.h>
#include <TimerEvent.h>
#include <FWActionSheet.h>
#include <EventLayout.h>

#include "FrontView.h"
#include "ImageDialog.h"
#include "ImageLoader.h"

#include <iostream>

#define ID_CLICK_ME_BUTTON 1
#define ID_SHOW_MENU_BUTTON 2
#define ID_SHOW_ACTIONSHEET 3
#define ID_OPEN_LINK 4
#define ID_PROGRESS_SPINNER 5
#define ID_HIDE_BUTTON 6

using namespace std;

Example1::Example1() : FWApplication("com.sometrik.example1")
{
  style("background", "#ffff00");

  createTimer(1000);
  
  actionBar = std::make_shared<ActionBar>();
  actionBar->setTitle("Pitkä yläotsikko tässä", "Alaotsikko");
  addChild(actionBar);
  
  navigationDrawer = std::make_shared<NavigationDrawer>();
  auto navigationLayout = std::make_shared<LinearLayout>(FW_VERTICAL);
  navigationDrawer->addChild(navigationLayout);
  navigationDrawer->style("background", "#cccccc");
  navigationLayout->addChild(make_shared<TextLabel>("Hello sidebar!"));
  navigationLayout->addChild(make_shared<Button>("OK"));
  auto & eventLayout = navigationLayout->addChild(make_shared<EventLayout>(1235));
  eventLayout.addChild(make_shared<TextLabel>("TEST TEST")).style("padding", 10);
  addChild(navigationDrawer);
   
  frontView = std::make_shared<FrontView>();
  addChild(frontView);

  mainView = std::make_shared<FrameView>("Secondary page");
  mainView->style("background", "#008000");
  addChild(mainView);

  auto flipper = std::make_shared<FlipperLayout>();
  // flipper->style("background-color", "#008080");
  mainView->addChild(flipper);

  auto navigationBar = std::make_shared<NavigationBar>();
  navigationBar->style("background-color", "#ff0000");
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 1")).style("icon", "icon1.png");
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 2")).style("icon", "icon2.png");
  navigationBar->addChild(std::make_shared<NavigationBarItem>("Page 3")).style("icon", "icon3.png");
  
  mainView->addChild(navigationBar);

#if 1
  auto firstPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  // firstPage->style("padding", 10);
  firstPage->style("border", 1);
  firstPage->style("background-color", "#800000");
  flipper->addChild(firstPage);
  
  auto image = std::make_shared<ImageElement>("test.png");
  image->style("width", 200);
  image->style("height", 200);
  image->style("gravity", "center");
  firstPage->addChild(image);
  
  auto titleLayout = make_shared<EventLayout>(1234);
  firstPage->addChild(titleLayout);
  titleLayout->style("padding", 10);
  titleLayout->style("border", 1);
  
  title = make_shared<TextLabel>("Hello again!");
  title->style("font-size", 20);
  title->style("padding", 20);
  title->style("background-color", "#e03030");
  title->style("shadow", 5);
  title->style("border", "#801010");
  title->style("border-radius", 5);
  titleLayout->addChild(title);

  auto nameLayout = make_shared<LinearLayout>(FW_HORIZONTAL);
  nameLayout->style("border", 1);
  nameLayout->style("width", "match-parent");
  nameLayout->style("margin", 10);
  nameLayout->style("background-color", "#008000");
  firstPage->addChild(nameLayout);
  nameLayout->addChild(make_shared<TextLabel>("Kirjoita nimesi:"));
  
  textField = make_shared<TextField>();
  nameLayout->addChild(textField);
  
  auto buttonLayout = std::make_shared<LinearLayout>(FW_HORIZONTAL);
  buttonLayout->style("border", 1);
  buttonLayout->addChild(make_shared<Button>("Click me!", ID_CLICK_ME_BUTTON)).style("background", "#30e030").style("border-radius", 5);
  buttonLayout->addChild(make_shared<Button>("Show menu", ID_SHOW_MENU_BUTTON));
  buttonLayout->addChild(make_shared<Button>("Actionsheet", ID_SHOW_ACTIONSHEET)).style("icon", "button.png");
  firstPage->addChild(buttonLayout);

  firstPage->addChild(make_shared<Button>("Open link", ID_OPEN_LINK));
  
  auto scrollLayout = std::make_shared<ScrollLayout>();
  scrollLayout->style("height", 200);
  scrollLayout->style("width", "match-parent");
  scrollLayout->style("background-color", "#808080");
  scrollLayout->style("border", "1");
  firstPage->addChild(scrollLayout);
  scrollContent = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout->addChild(scrollContent);
  
  for (int i = 0; i < 100; i++) {
    auto label = std::make_shared<TextLabel>("Number: " + to_string(i));
    label->style("white-space", "nowrap");
    scrollContent->addChild(label);
  }
  
  firstPage->addChild(make_shared<Switch>("On", "Off"));

  auto secondPage = std::make_shared<FrameLayout>();
  secondPage->style("background-color", "#e03030");
  flipper->addChild(secondPage);
    
  auto scrollLayout2 = std::make_shared<ScrollLayout>();
  scrollLayout2->style("border", "1");
  scrollLayout2->style("height", "match-parent");
  scrollLayout2->style("width", "match-parent");
  secondPage->addChild(scrollLayout2);
  auto scrollContent2 = std::make_shared<LinearLayout>(FW_VERTICAL);
  scrollLayout2->addChild(scrollContent2);
  
  for (char c = 'A'; c <= 'Z'; c++) {
    string s;
    s += c;
    scrollContent2->addChild(make_shared<TextLabel>(s)).style("font-size", 80);
  }
#endif

  auto thirdPage = std::make_shared<LinearLayout>(FW_VERTICAL);
  thirdPage->style("background", "#ff0000");
  thirdPage->style("margin", 20);
  thirdPage->style("border", 1);

  flipper->addChild(thirdPage);
  thirdPage->addChild(make_shared<TextLabel>("THIRD PAGE")).style("font-size", "20");
  thirdPage->addChild(make_shared<ProgressSpinner>(ID_PROGRESS_SPINNER));
  thirdPage->addChild(make_shared<Button>("Hide spinner", ID_HIDE_BUTTON));

}

void
Example1::onCommandEvent(CommandEvent & ev) {
  cerr << "got command event: " << ev.getElementId() << endl;
  switch (ev.getElementId()) {
    case ID_CLICK_ME_BUTTON: {
      cerr << "showing dialog" << endl;
      auto dialog = make_shared<ImageDialog>();
      showModal(dialog);
    }
      break;
    case ID_SHOW_MENU_BUTTON: {
      cerr << "showing menu" << endl;
      navigationDrawer->show();
    }
      break;
    case ID_SHOW_FRONT_PAGE: {
      actionBar->setTitle("Yläotsikko ilman alaotsikkoa", "");
      mainView->show();
    }
      break;
    case ID_SHOW_ACTIONSHEET: {
      auto sheet = make_shared<FWActionSheet>("Test");
      sheet->addOption(1, "Do something");
      sheet->addOption(2, "Do something else");
      int r = showModal(sheet);
      cerr << "Actionsheet returned with " << r << endl;
      if (r == 1) {
	launchBrowser("http://www.slashdot.org/");
      } else if (r == 2) {
	launchBrowser("http://www.reddit.com/");
      }
    }
      break;
      
    case ID_OPEN_LINK: {
      launchBrowser("http://www.yle.fi/");
    }
      break;
      
    case ID_HIDE_BUTTON: {
      getChildById(ID_PROGRESS_SPINNER).hide();
    }
      break;
  }
}

void
Example1::onTimerEvent(TimerEvent & ev) {
  auto & children = scrollContent->getChildren();
  scrollContent->reorderChildren(*(children.back()), 0);
  children.insert(children.begin(), children.back());
  children.pop_back();
  
  title->text(to_string(counter++));
}

void
Example1::handleImageRequestEvent(ImageRequestEvent & ev) {
  cerr << "received image request, url = " << ev.getImageUrl() << endl;
  if (ev.getImageUrl().empty()) {
    string url = "https://picsum.photos/" + to_string(ev.getRequestedWidth()) + "/" + to_string(ev.getRequestedHeight()) + "/?random";
    ev.setImageUrl(url);
  }
  auto loader = make_shared<ImageLoader>(ev, "example1");
  getThread().run(loader);
}

FWApplication * applicationMain() {
  return new Example1();  
}
