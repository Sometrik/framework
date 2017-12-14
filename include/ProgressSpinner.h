#ifndef _PROGRESS_SPINNER_H_
#define _PROGRESS_SPINNER_H_

#include <Element.h>

class ProgressSpinner : public Element {
 public:
  ProgressSpinner(int _id = 0, unsigned int _flags = 0)
   : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "ProgressSpinner") return true;
    return Element::isA(className);
  }

protected:
  void create() override {
    Command c(Command::CREATE_PROGRESS_SPINNER, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
};

#endif
