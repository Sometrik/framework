#include <Element.h>
#include <FWApplication.h>
#include <ElementNotInitializedException.h>
#include <PlatformThread.h>
#include <TextLabel.h>

#include <cassert>
#include <typeinfo>

using namespace std;

std::unordered_map<int, Element *> Element::registered_elements;
std::unordered_set<int> Element::network_activity;
Mutex Element::mutex;

Element::~Element() {
  if (unregisterElement(this)) {
    if (auto ptr = thread.lock()) {
      Command c(Command::DELETE_ELEMENT, getInternalId());
      ptr->sendCommand(c);
    }
  }
}

void
Element::initialize(std::shared_ptr<PlatformThread> _thread) {
  assert(_thread.get());
  if (_thread.get()) {
    thread = _thread;
    registerElement(this);
    prepare();
    initializeContent();

    for (auto & c : getChildren()) {
      c->initialize(_thread);
    }
    
    load();
  }
}

bool
Element::initializeContent() {
  if (!is_content_initialized &&
      (!parent || parent->is_content_initialized) &&
      (isVisible() || (parent && parent->isVisible()))) {

    if (auto t = thread.lock()) {
      is_content_initialized = true;

      create();

      if (!pendingCommands.empty()) {
	if (t->isInTransaction()) {
	  for (auto & c : pendingCommands) {
	    t->sendCommand(c);
	  }
	} else {
	  t->sendCommands(pendingCommands);
	}
	pendingCommands.clear();
      }

      return true;
    }
  }
  return false;
}

void
Element::initializeChildContent() {
  if (isVisible()) {
    if (is_content_initialized) {
      for (auto & c : getChildren()) {
	c->initializeContent();
	c->initializeChildContent();
      }
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

  initializeContent();
  initializeChildContent();
  
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
Element::stop() {
  sendCommand(Command(Command::STOP, getInternalId()));
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
  if (auto ptr = thread.lock()) {
    ptr->beginBatch();
  }
}

void
Element::commit() {
  if (auto ptr = thread.lock()) {
    ptr->commitBatch();
  }
}

void
Element::sendCommand(const Command & command) {
  if (is_content_initialized) {
    if (auto ptr = thread.lock()) {
      ptr->sendCommand(command);
      return;
    }
  }
  pendingCommands.push_back(command);
}

void
Element::onEvent(Event & ev) {
  if (!ev.isHandled() && ev.getTTL()) {
    ev.decTTL();
    if (ev.isBroadcast()) {
      for (auto & c : getChildren()){
	ev.dispatch(*c);
      }
    } else if (parent) {
      ev.dispatch(*parent);
    }
    ev.incTTL();
  }
}

void
Element::reorderChildren(Element & child, unsigned int new_position) {
  Command c(Command::REORDER_CHILD, getInternalId(), child.getInternalId());
  c.setValue(new_position);
  sendCommand(c);
  
  std::shared_ptr<Element> e;
  for (auto it = children.begin(); it != children.end(); it++) {
    assert(it->get());
    if ((*it)->getInternalId() == child.getInternalId()) {
      e = *it;
      children.erase(it);
      break;
    }
  }
  if (e) {
    children.insert(children.begin() + new_position, e);
  }
}

void
Element::showToast(const std::string & message, int duration) {
  Command c(Command::CREATE_TOAST, getInternalId());
  c.setTextValue(message);
  c.setValue(duration);
  sendCommand(c);
}

void
Element::launchBrowser(const std::string & input_url) {
  sendCommand(Command(Command::LAUNCH_BROWSER, getInternalId(), input_url));
}

Element &
Element::addChild(const std::shared_ptr<Element> & element) {
  assert(element);
  
  element->setParent(this);
  children.push_back(element);

  if (auto ptr = thread.lock()) {
    element->initialize(ptr);
  }
  return *element;
}

Element &
Element::addChild(const std::string & text) {
  return addChild(std::make_shared<TextLabel>(text));
}


FWApplication &
Element::getApplication() {
  if (auto ptr = thread.lock()) {
    return ptr->getApplication();
  } else {
    throw ElementNotInitializedException();
  }
}

const FWApplication &
Element::getApplication() const {
  if (auto ptr = thread.lock()) {
    return ptr->getApplication();
  } else {
    throw ElementNotInitializedException();
  }
}

void
Element::removeChildren() {
  for (auto & child : children) {
    sendCommand(Command(Command::REMOVE_CHILD, getInternalId(), child->getInternalId()));
  }
  children.clear();
}

void
Element::removeChild(Element * child) {
  if (child) {
    child->setParent(0);
    for (auto it = children.begin(); it != children.end(); it++) {
      if (it->get() == child) {
        sendCommand(Command(Command::REMOVE_CHILD, getInternalId(), child->getInternalId()));
        children.erase(it);
        return;
      }
    }
  }
}

void
Element::removeFromParent() {
  if (parent) {
    parent->removeChild(this);
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

int
Element::showModal(const std::shared_ptr<Element> & dialog) {
  addChild(dialog);
  if (auto ptr = thread.lock()) {
    return ptr->startModal();
  } else {
    return 0;
  }
}
