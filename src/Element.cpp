#include <Element.h>
#include <FWPlatform.h>
#include <TextLabel.h>
#include <LinearLayout.h>

using namespace std;

void
Element::initialize(FWPlatform * _platform) {
  platform = _platform;
  internal_id = platform->getNextInternalId();
}

void
Element::sendMessage(const Message & message){
  platform->sendMessage(message);
}

Element &
Element::addChild(const std::string & text) {
  return addChild(make_shared<TextLabel>(text));
}

LinearLayout &
Element::addHorizontalLayout() {
  auto l = make_shared<LinearLayout>(FW_HORIZONTAL);
  addChild(l);
  return *l;
}

LinearLayout &
Element::addVerticalLayout() {
  auto l = make_shared<LinearLayout>(FW_VERTICAL);
  addChild(l);
  return *l;
}

void
Element::onEvent(EventBase & ev) {
  if (parent && !ev.isHandled()) {
    ev.dispatch(*parent);
  }
}
