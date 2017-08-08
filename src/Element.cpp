#include <Element.h>
#include <FWPlatform.h>
#include <HeadingText.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <FWApplication.h>

#include <cassert>

using namespace std;

atomic<int> Element::nextInternalId(1);

Element::~Element() {
  // the platform itself cannot be unregistered at this point
  if (platform && this != platform) {
    platform->unregisterElement(this);
  }
}

void
Element::initialize(FWPlatform * _platform) {
  assert(_platform);
  if (_platform) {
    platform = _platform;
    platform->registerElement(this);
    create();
    for (auto & c : pendingCommands) {
      sendCommand(c);
    }
    pendingCommands.clear();
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
Element::setEnabled(bool _is_enabled) {
  is_enabled = _is_enabled;
  Command c(Command::SET_ENABLED, getInternalId());
  c.setValue(is_enabled ? 1 : 0);
  sendCommand(c);
}

void
Element::show() {
  is_visible = true;
  Command c(Command::SET_VISIBILITY, getInternalId());
  c.setValue(1);
  sendCommand(c);
}

void
Element::hide() {
  is_visible = false;
  Command c(Command::SET_VISIBILITY, getInternalId());
  c.setValue(0);
  sendCommand(c);
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
  if (platform) {
    platform->sendCommand2(command);
  } else {
    pendingCommands.push_back(command);
  }
}

Element &
Element::addHeading(const std::string & text) {
  return addChild(make_shared<HeadingText>(text));
}

Element &
Element::addText(const std::string & text) {
  return addChild(make_shared<TextLabel>(text));
}

Element &
Element::addHorizontalLayout(int _id) {
  return addChild(make_shared<LinearLayout>(FW_HORIZONTAL, _id));
}

Element &
Element::addVerticalLayout(int _id) {
  return addChild(make_shared<LinearLayout>(FW_VERTICAL, _id));
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

void
Element::showToast(const std::string & message, int duration) {
  Command c(Command::CREATE_TOAST, getParentInternalId(), getInternalId());
  c.setTextValue(message);
  c.setValue(duration);
  sendCommand(c);
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

void
Element::removeChild(Element * child) {
  for (auto it = children.begin(); it != children.end(); it++) {
    if (it->get() == child) {
      sendCommand(Command(Command::DELETE_ELEMENT, getInternalId(), child->getInternalId()));
      children.erase(it);
      return;
    }
  }
}

int
Element::createTimer(int timeout_ms) {
  int timer_id = getNextInternalId();
  sendCommand(Command(Command::CREATE_TIMER, getInternalId(), timer_id));
  return timer_id;
}
