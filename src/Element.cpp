#include <Element.h>
#include <FWPlatform.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Command.h>

#include <cassert>

using namespace std;

Element::~Element() {
  if (isInitialized()) {
    sendCommand(Command(Command::DELETE_ELEMENT, getParentInternalId(), getInternalId()));
  }
}

void
Element::initialize(FWPlatform * _platform) {
  platform = _platform;
  internal_id = platform->getNextInternalId();
}

void
Element::sendCommand(const Command & command){
  assert(this != platform);
  platform->sendCommand(command);
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

void
Element::showMessageDialog(const std::string & title, const std::string & text) {
  Command c(Command::SHOW_MESSAGE_DIALOG, getParentInternalId(), getInternalId());
  c.setTextValue(title);
  c.setTextValue2(text);
  sendCommand(c);
}

std::string
Element::showInputDialog(const std::string & title, const std::string & text) {
  Command c(Command::SHOW_INPUT_DIALOG, getParentInternalId(), getInternalId());
  c.setTextValue(title);
  c.setTextValue2(text);
  sendCommand(c);
  return platform->getModalResultText();
}
