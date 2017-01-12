#ifndef _INPUTELEMENT_H_
#define _INPUTELEMENT_H_

#include <Element.h>

class InputElement : public Element {
 public:
  InputElement() { }

  template<class T>
  void notify(T arg) {
    for (auto & o : getBoundObjects()) {
      o->call(arg);
    }
  }
};

#endif
