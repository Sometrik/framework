#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Message.h>

class ImageElement : public Element {
 public:
  ImageElement(int _width, int _height) : width(_width), height(_height) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Message m(Message::CREATE_IMAGE_ELEMENT, getParentInternalId(), getInternalId());
    m.setWidth(width);
    m.setHeigh(height);
    sendMessage(m);
  }

 private:
  int width, height;
};

#endif
