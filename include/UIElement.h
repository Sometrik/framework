#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <Element.h>
#include <Context.h>
#include <TouchEvent.h>
#include <CommandEvent.h>
#include <SysEvent.h>
#include <FWPlatform.h>
#include <GraphicsContext.h>

#include <cassert>
 
class UIElement : public Element {
 public:
  UIElement(int _id = 0, unsigned int _flags = 0)
    : Element(_id, _flags) { }

  bool isA(const std::string & className) const override {
    if (className == "UIElement") return true;
    return Element::isA(className);
  }

  void setX(float _x) { x = _x; }
  void setY(float _y) { y = _y; }
  void setWidth(float _width) { width = _width; }
  void setHeight(float _height) { height = _height; }

  void onDrawEvent(DrawEvent & ev) override {
    if (context) {
      if (!context->hasContent()) {
	renderContent();
      }
      context->draw(x, y, width, height);
    }
  }

  void onSysEvent(SysEvent & ev) {
    if (ev.getType() == SysEvent::STOP && context) {
      context->clear();
    }
  }

  void setTouched(bool t) {
    if (t != is_touched) {
      is_touched = t;
      if (context) context->clear();
    }
  }

  bool isTouched() const { return is_touched; }

 protected:
  virtual void renderContent() = 0;

  float x = 0, y = 0, width = 0, height = 0;
  bool is_touched = false;
  GraphicsContext * context = 0;
};

#endif
