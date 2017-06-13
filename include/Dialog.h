#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Element.h>
#include <Command.h>
#include <FWPlatform.h>
#include <ValueEvent.h>
#include <CommandEvent.h>

class Dialog : public Element {
 public:
  Dialog(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "Dialog") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(ev.getTimestamp(), getId());
    ev2.dispatch(*this);
  }

  int showModal(Element * parent) {
    if (!isInitialized()) {
      setParent(parent);
      initialize(&(parent->getPlatform()));
      initializeChildren();
    }
    sendCommand(Command(Command::SHOW_DIALOG, getInternalId()));
    return getPlatform().getModalResultValue();
  }

protected:
  void create() override {
    Command c(Command::CREATE_DIALOG, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
  }
};

#endif
