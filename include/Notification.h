#ifndef _NOTIFICATION_H_
#define _NOTIFICATION_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class Notification : public Element {
 public:
  Notification(std::string _title, std::string _text, int _id = 0) : Element(_id), title(_title), text(_text) { }

  bool isA(const std::string & className) const override {
    if (className == "Notification") return true;
    return Element::isA(className);
  }

//  void onValueEvent(ValueEvent & ev) override {
//     notify();
//     CommandEvent ev2(getId());
//     ev2.dispatch(*this);
//   }

 protected:
  void create() override {
    Command c(Command::CREATE_NOTIFICATION, getParentInternalId(), getInternalId());
    c.setTextValue(title);
    c.setTextValue2(text);
    sendCommand(c);
  }
			       
 private:
  std::string title, text;
};

#endif
