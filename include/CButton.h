#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include <CanvasElement.h>

class CButton : public CanvasElement {
 public:
  CButton(const std::string & _label) : label(_label) { }

 private:
  std::string label;
};

#endif
