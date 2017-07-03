#ifndef _TOAST_H_
#define _TOAST_H_

#include <Element.h>
#include <Command.h>
#include <FWPlatform.h>

class Toast : public Element {
 public:
  Toast(const std::string & _message, int _seconds = 2, int _id = 0)
    : Element(_id), message(_message), seconds(_seconds) { }

  bool isA(const std::string & className) const override {
    if (className == "Toast") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_TOAST, getParentInternalId(), getInternalId());
    c.setTextValue(message);
    c.setValue(seconds);
    sendCommand(c);
  }

 private:
  std::string message;
  int seconds;
};

#endif
