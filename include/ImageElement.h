#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

#include <ImageResponseEvent.h>

class ImageElement : public Element {
 public:
 ImageElement(const std::string & _filename, int _id = 0) : Element(_id), filename(_filename) { }

  bool isA(const std::string & className) const override {
    if (className == "ImageElement") return true;
    return Element::isA(className);
  }

  void handleImageResponseEvent(ImageResponseEvent & ev) override {
    Command c(Command::SET_IMAGE, getInternalId());
    auto & image = ev.getImage();
    std::string data2((const char *)image->getData(), image->calculateSize());
    c.setTextValue(data2);
    c.setWidth(image->getWidth());
    c.setHeight(image->getHeight());
    c.setValue(int(image->getInternalFormat()));
    sendCommand(c);
  }

  void setImageFile(std::string _filename){
    filename = _filename;

    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(filename);
    sendCommand(c);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  const std::string & getFilename() { return filename; }
  void setFilename(const std::string & _filename) { filename = _filename; }
  
 protected:
  void create() override {
    Command c(Command::CREATE_IMAGEVIEW, getParentInternalId(), getInternalId());
    c.setTextValue(filename);
    sendCommand(c);
  }

 private:
  std::string filename;
};

#endif
