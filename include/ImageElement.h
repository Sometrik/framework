#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>

class ImageElement : public Element {
 public:
  ImageElement(int _width, int _height) : width(_width), height(_height) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_IMAGE_ELEMENT, getParentInternalId(), getInternalId());
    c.setWidth(width);
    c.setHeight(height);
    sendMessage(c);
  }

 private:
  int width, height;
};

#endif
