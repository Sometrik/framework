#include <Element.h>
#include <FWPlatform.h>
#include <TextLabel.h>
#include <LinearLayout.h>
#include <FWApplication.h>
#include <ElementNotInitializedException.h>
#include <PlatformThread.h>

#include <cassert>

using namespace std;

atomic<int> Element::nextInternalId(1);

Element::~Element() {
  if (thread) {
    auto & platform = thread->getPlatform();
    platform.unregisterElement(this);
  }
}

void
Element::initialize(PlatformThread * _thread) {
  assert(_thread);
  if (_thread) {
    thread = _thread;
    thread->getPlatform().registerElement(this);
    create();
    load();
    if (!pendingCommands.empty()) {
      if (thread->isInTransaction()) {
        for (auto & c : pendingCommands) {
          thread->sendCommand(c);
        }
      } else {
        thread->sendCommands(pendingCommands);
      }
      pendingCommands.clear();
    }
  }
  assert(isInitialized());
}

void
Element::initializeChildren() {
  assert(isInitialized());
  if (isInitialized()) {
    for (auto & c : getChildren()) {
      c->initialize(thread);
      c->initializeChildren();
    }
  }
}

void
Element::setError(bool t) {
  if ((t && !has_error) || (!t && has_error)) {
    has_error = t;
    Command c(Command::SET_ERROR, getInternalId());
    c.setValue(t ? 1 : 0);
    if (t) c.setTextValue("Arvo ei kelpaa!");
    sendCommand(c);
  }
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

Element &
Element::style(Selector s, const std::string & key, const std::string & value) {
  Command c(Command::SET_STYLE, getInternalId());
  c.setValue(int(s));
  c.setTextValue(key);
  c.setTextValue2(value);
  sendCommand(c);
  return *this;
}

void
Element::begin() {
  if (thread) thread->beginBatch();
}

void
Element::commit() {
  if (thread) thread->commitBatch();
}

int
Element::sendCommand(const Command & command) {
  if (thread) {
    return thread->sendCommand(command);
  } else {
    pendingCommands.push_back(command);
    return 0;
  }
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
  if (sendCommand(c) && thread) {
    return thread->getModalResultText();
  } else {
    return "";
  }
}

void
Element::showToast(const std::string & message, int duration) {
  Command c(Command::CREATE_TOAST, getParentInternalId(), getInternalId());
  c.setTextValue(message);
  c.setValue(duration);
  sendCommand(c);
}

void
Element::launchBrowser(const std::string & input_url) {
  sendCommand(Command(Command::LAUNCH_BROWSER, getInternalId(), input_url));
}

FWApplication &
Element::getApplication() {
  if (!thread) throw ElementNotInitializedException();
  return thread->getApplication();
}

const FWApplication &
Element::getApplication() const {
  if (!thread) throw ElementNotInitializedException();
  return thread->getApplication();
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
  Command c(Command::CREATE_TIMER, getInternalId(), timer_id);
  c.setValue(timeout_ms);
  sendCommand(c);

  return timer_id;
}
