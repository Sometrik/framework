#ifndef _PROGRESSBAR_H_
#define _PROGRESSBAR_H_

#include <UIElement.h>

class ProgressBar : public Element {
 public:
  ProgressBar(int _id = 0, unsigned int _flags = 0)
   : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "ProgressBar") return true;
    return Element::isA(className);
  }

protected:
  void create() override {
    Command c(Command::CREATE_PROGRESSBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
};

#endif
