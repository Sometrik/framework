#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>

class ImageElement : public Element {
 public:
 ImageElement(const std::string & _filename) : filename(_filename) { }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_IMAGE_ELEMENT, getParentInternalId(), getInternalId());
    c.setTextValue(filename);
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);
  }

 private:
  std::string filename;
};

#endif
