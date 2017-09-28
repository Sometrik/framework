#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Element.h>
#include <Command.h>
#include <FWPlatform.h>
#include <ValueEvent.h>
#include <CommandEvent.h>

class Dialog : public Element {
 public:
  Dialog(const std::string & _title, int _id = 0)
    : Element(_id), title(_title) { }

  bool isA(const std::string & className) const override {
    if (className == "Dialog") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId());
    ev2.dispatch(*this);
  }

  int showModal(Element * parent) override {
    if (!isInitialized()) {
      setParent(parent);
      initialize(&(parent->getPlatform()));
      initializeChildren();
      load();
    }
    sendCommand(Command(Command::SHOW_DIALOG, getInternalId()));
    return getPlatform().getModalResultValue();
  }

 protected:
  void endModal(int value = 0) {
    sendCommand(Command(Command::END_MODAL, getInternalId()));
  }
  
  void create() override {
    Command c(Command::CREATE_DIALOG, getParentInternalId(), getInternalId());
    c.setTextValue(title);
    sendCommand(c);
  }

 private:
  std::string title;
};

#endif
