#ifndef _PAGE_CONTROL_H_
#define _PAGE_CONTROL_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class PageControl : public Element {
 public:
  PageControl(int _size, int _id = 0) : Element(_id), size(_size) { }

  bool isA(const std::string & className) const override {
    if (className == "PageControl") return true;
    return Element::isA(className);
  }

  void setActiveIndicator(int value) {
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(value);
    sendCommand(c);
  }

  void reshape(size_t value) {
    Command c(Command::RESHAPE_TABLE, getInternalId());
    c.setValue((int)value);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_PAGE_CONTROL, getParentInternalId(), getInternalId());
    c.setValue(size);
    sendCommand(c);
  }

 private:
  int size;
};

#endif
