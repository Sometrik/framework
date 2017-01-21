#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <UIElement.h>

#include <ValueEvent.h>
#include <Command.h>
#include <CommandEvent.h>
#include <TouchEvent.h>
#include <FWPlatform.h>
#include <PrimitiveRenderer.h>
#include <OpenGLTexture.h>

class Button : public UIElement {
 public:
  Button(const std::string & _label) : label(_label) { }

  bool isA(const std::string & className) override {
    if (className == "Button") return true;
    return UIElement::isA(className);
  }

  void onTouchEvent(TouchEvent & ev) override {
    if (ev.getType() == TouchEvent::ACTION_DOWN &&
	ev.getX() >= x && ev.getX() < x + width &&
	ev.getY() >= y && ev.getY() < y + height) {
      setTouched(true);
      CommandEvent ev2(ev.getTimestamp(), getId());
      ev2.dispatch(*this);
      ev.setHandled();
      ev.requestRedraw();
    } else if (ev.getType() == TouchEvent::ACTION_UP && isTouched()) {
      setTouched(false);
      ev.setHandled();
      ev.requestRedraw();
    }
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(ev.getTimestamp(), getId());
    ev2.dispatch(*this);
  }
  
  std::shared_ptr<canvas::Texture> drawContent() override {
    auto context = getPlatform().createContextFactory()->createContext(width, height, canvas::InternalFormat::RGBA8);

    float x = 0.0f, y = 0.0f, w = width - 1, h = height - 1, r = 5.0f;
    context->moveTo(x+r, y);
    context->arcTo(x+w, y,   x+w, y+h, r);
    context->arcTo(x+w, y+h, x,   y+h, r);
    context->arcTo(x,   y+h, x,   y,   r);
    context->arcTo(x,   y,   x+w, y,   r);
    if (is_touched) {
      context->fillStyle = "#730";
      context->fill();
    }
    context->strokeStyle = "#e80";
    context->stroke();
    context->fillStyle = "#fff";
    context->font.size = 20;
    context->textAlign = "center";
    context->textBaseline = "middle";
    context->fillText(label, width / 2, height / 2);    
    return canvas::OpenGLTexture::createTexture(context->getDefaultSurface());
  }

  void setLabel(const std::string & l) {
    if (l != label) {
      label = l;
      clearTexture();
    }
  }

  bool call() override {
    sendCommand(Command(Command::SET_INT_VALUE, getInternalId()));
    return true;
  }
  bool call(bool t) override { return call(); }
  bool call(int i) override { return call(); }
  bool call(const std::string & s) override { return call(); }

 protected: 
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_BUTTON, getParentInternalId(), getInternalId());
    c.setTextValue(label);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  std::string label;
};

#endif
