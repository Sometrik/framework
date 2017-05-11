#ifndef _FWVIEW_H_
#define _FWVIEW_H_

#include <Element.h>
#include <Command.h>

class FWViewBase : public Element {
 public:
  FWViewBase(int _id) : Element(_id) { }
  FWViewBase(const std::string & _label, int _id)
    : Element(_id), label(_label) { }

  bool isA(const std::string & className) override {
    if (className == "FWViewBase") return true;
    return Element::isA(className);
  }

  void show(bool recordHistory = true) override {
    Command c(Command::SET_INT_VALUE, getInternalId());
    if (recordHistory){
      c.setValue(1);
    } else {
      c.setValue(2);
    }
    sendCommand(c);
  }

  const std::string & getLabel() const { return label; }
  
 private:
  std::string label;
};

#endif
