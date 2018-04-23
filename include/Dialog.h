#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Element.h>
#include <Command.h>
#include <ValueEvent.h>
#include <PlatformThread.h>

class Dialog : public Element {
 public:
  Dialog(const std::string & _title, int _id = 0)
    : Element(_id), title(_title) { }

  bool isA(const std::string & className) const override {
    if (className == "Dialog") return true;
    return Element::isA(className);
  }

  int showModal(Element * parent) {
    if (!isInitialized()) {
      setParent(parent);
      initialize(&(parent->getThread()));
      initializeChildren();
    }
    return getThread().startModal();
  }

  void onValueEvent(ValueEvent & ev) override {
    ev.setHandled(true);
    endModal(ev.getValue());
  }

  const std::string & getTitle() const { return title; }

  void setTitle(const std::string & _title) {
    title = _title;
    if (!isInitialized()) {
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setTextValue(title);
      sendCommand(c);
    }
  }

 protected:
  void endModal(int value = 0) {
    getThread().endModal(value);
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
