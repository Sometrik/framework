#ifndef _FORMVIEW_H_
#define _FORMVIEW_H_

#include <FWViewBase.h>
#include <Command.h>

class FormView : public FWViewBase {
 public:
  FormView(int _id = 0) : FWViewBase(_id) { }
  FormView(const std::string & label, int _id = 0) : FWViewBase(label, _id) { }

  bool isA(const std::string & className) const override {
    if (className == "FormView") return true;
    return Element::isA(className);
  }

 protected:
  void create() override {
    Command c(Command::CREATE_FORMVIEW, getParentInternalId(), getInternalId());
    c.setTextValue(getTitle());
    sendCommand(c);
  }
};

#endif
