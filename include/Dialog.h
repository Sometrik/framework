#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Element.h>
#include <Command.h>
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

  int showModal(Element * parent) override {
    if (!isInitialized()) {
      setParent(parent);
      initialize(&(parent->getThread()));
      initializeChildren();
    }
    return sendCommand(Command(Command::SHOW_MODAL, getInternalId()));
  }

 protected:
  void endModal(int value = 0) {
    Command c(Command::END_MODAL, getInternalId());
    c.setValue(value);
    sendCommand(c);
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
