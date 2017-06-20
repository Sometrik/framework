#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>
#include <Command.h>

class FWViewBase : public Element {
 public:
  FWViewBase(int _id) : Element(_id) { }
  FWViewBase(const std::string & _title, int _id)
    : Element(_id), title(_title) { }

  bool isA(const std::string & className) const override {
    if (className == "FWViewBase") return true;
    return Element::isA(className);
  }

  void show() override {
    Element::show();
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(1);
    c.setTextValue(getTitle());
    sendCommand(c);
  }

  virtual std::string getTitle() const { return title; }
    
 private:
  std::string title;
};

#endif
