#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>
#include <Command.h>
#include <FWApplication.h>

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
    
    Command c1(Command::SET_INT_VALUE, getInternalId());
    c1.setValue(1);
    c1.setTextValue(getTitle());
    sendCommand(c1);

    Command c2(Command::SET_INT_VALUE, getApplication().getInternalId(), getInternalId());
    c2.setValue(1);
    c2.setTextValue(getTitle());
    sendCommand(c2);
  }

  virtual std::string getTitle() const { return title; }
    
 private:
  std::string title;
};

#endif
