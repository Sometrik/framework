#ifndef _INPUTELEMENT_H_
#define _INPUTELEMENT_H_

#include <Element.h>

class InputElement : public Element {
 public:
  InputElement() { }

  bool isA(const std::string & className) override {
    if (className == "InputElement") return true;
    return Element::isA(className);
  }
};

#endif
