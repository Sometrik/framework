#ifndef _CBUTTON_H_
#define _CBUTTON_H_

#include <CanvasElement.h>
#include <PrimitiveRenderer.h>

class CButton : public CanvasElement {
 public:
  CButton(const std::string & _label) : label(_label) { }

 private:
  std::string label;
  float highlight = 0.0f;
};

#endif
