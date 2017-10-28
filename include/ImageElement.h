#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>
#include <ImageSet.h>

#include <ImageResponseEvent.h>

class ImageElement : public Element {
 public:
  ImageElement(const ImageSet & _images, int _id = 0) : Element(_id), images(_images) { }
  ImageElement(const std::string & _filename, int _id = 0) : Element(_id) {
    setImageFile(_filename);
  }

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

  void clear() {
    images.clear();
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

  void setImageFile(const std::string & _filename) {
    ImageSet _images;
    _images.insert(_filename);
    setData(_images);    
  }

  void setData(const ImageSet & _images) {
    clear();
    images = _images;

    for (auto it = images.begin(); it != images.end(); it++) {
      Command c(Command::SET_TEXT_VALUE, getInternalId());
      c.setWidth(it->width);
      c.setHeight(it->height);
      c.setTextValue(it->url);
      sendCommand(c);
    }
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(getId());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void setFilename(const std::string & _filename) {
    data.clear();
    data.insert(_filename);
  }

  const ImageSet & getImages() const { return images; }
  
 protected:
  void create() override {
    Command c(Command::CREATE_IMAGEVIEW, getParentInternalId(), getInternalId());
    if (images.empty()) {
      sendCommand(c);
    } else {
      auto it = images.data().begin();
      c.setTextValue(it->url);
      c.setWidth(it->width);
      c.setHeight(it->height);
      sendCommand(c);

      for (it++; it != images.end(); it++) {
	Command c(Command::SET_TEXT_VALUE, getInternalId());
	c.setTextValue(it->url);
	c.setWidth(it->width);
	c.setHeight(it->height);
	sendCommand(c);	
      }
    }
  }

 private:
  ImageSet images;
};

#endif
