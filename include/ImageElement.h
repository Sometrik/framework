#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>

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
    auto & data = ev.getImage()->getData();
    std::string data2((const char *)data.getData(), data.calculateSize());
    c.setTextValue(data2);
    c.setWidth(ev.getImage()->getData().getWidth());
    c.setHeight(ev.getImage()->getData().getHeight());
    c.setValue(int(ev.getImage()->getData().getInternalFormat()));
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
  }

  const std::string & getFilename() { return filename; }
  void setFilename(const std::string & _filename) { filename = filename; }
  
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
