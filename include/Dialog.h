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

  bool isVisible() const override {
    return true;
  }

 protected:
  void endModal(int value = 0) {
    auto t = getThreadPtr();
    if (t) t->endModal(value);
    getParent()->removeChild(this);
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
