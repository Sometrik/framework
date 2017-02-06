#include <Element.h>
#include <FWPlatform.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <Command.h>
#include <FWApplication.h>

#include <cassert>

using namespace std;

void
Element::initialize(FWPlatform * _platform) {
  assert(_platform);
  if (_platform) {
    platform = _platform;
    internal_id = platform->getNextInternalId();
  }
  assert(isInitialized());
}

void
Element::initializeChildren() {
  assert(isInitialized());
  if (isInitialized()) {
    for (auto & c : getChildren()) {
      c->initialize(platform);
      c->initializeChildren();
    }    
  }
}

void
Element::setError(bool t) {
  if ((t && !has_error) || (!t && has_error)) {
    has_error = t;
    cerr << "setting error to " << has_error << endl;
    Command c(Command::SET_ERROR, getInternalId());
    c.setValue(t ? 1 : 0);
    if (t) c.setTextValue("Arvo ei kelpaa!");
    sendCommand(c);
  }
}

void
Element::style(const std::string & key, const std::string & value) {
  Command c(Command::SET_STYLE, getInternalId());
  c.setTextValue(key);
  c.setTextValue2(value);
  sendCommand(c);
}
  
void
Element::sendCommand(const Command & command) {
  assert(platform);
  platform->sendCommand2(command);
}

Element &
Element::addHeading(const std::string & text) {
  return addChild(make_shared<HeadingText>(text));
}

Element &
Element::addText(const std::string & text) {
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
Element::onEvent(Event & ev) {
  if (!ev.isHandled()) {
    if (ev.isBroadcast()) {
      for (auto & c : getChildren()){
	ev.dispatch(*c);
      }
    } else if (parent) {
      ev.dispatch(*parent);
    }
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

FWApplication &
Element::getApplication() {
  auto p = getPlatform().getFirstChild();
  return dynamic_cast<FWApplication&>(*p);
}

const FWApplication &
Element::getApplication() const {
  auto p = getPlatform().getFirstChild();
  return dynamic_cast<const FWApplication&>(*p);
}

