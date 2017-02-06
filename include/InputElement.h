#ifndef _INPUTELEMENT_H_
#define _INPUTELEMENT_H_

#include <Element.h>

class InputElement : public Element {
 public:
 InputElement(int _id = 0, unsigned int _flags = 0) : Element(_id, _flags) { }

  bool isA(const std::string & className) override {
    if (className == "InputElement") return true;
    return Element::isA(className);
  }
};

#endif
