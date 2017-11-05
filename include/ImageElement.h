#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>
#include <ImageSet.h>
#include <PlatformThread.h>

#include <ImageResponseEvent.h>

class ImageElement : public Element {
 public:
  ImageElement(const ImageSet & _images, int _id = 0) : Element(_id), images(_images) { }
  ImageElement(const std::string & filename, int _id = 0) : Element(_id) {
    images.insert(filename);
  }

  bool isA(const std::string & className) const override {
    if (className == "ImageElement") return true;
    return Element::isA(className);
  }

  void handleImageResponseEvent(ImageResponseEvent & ev) override {
    if (ev.isSuccess()) {
      getThread().setImageData(getInternalId(), ev.getImage());
    }
    ev.setHandled(true);
  }

  void clear() {
    images.clear();
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

  void setImageFile(const std::string & filename) {
    images.clear();
    images.insert(filename);
    Command c(Command::SET_TEXT_VALUE, getInternalId());
    c.setTextValue(filename);
    sendCommand(c);
  }

  void setData(const ImageSet & _images) {
    clear();
    images = _images;

    for (auto it = images.data().begin(); it != images.data().end(); it++) {
      Command c(Command::ADD_IMAGE_URL, getInternalId());
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

#if 0
  void setFilename(const std::string & _filename) {
    images.clear();
    images.insert(_filename);
  }
#endif
  
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

      for (it++; it != images.data().end(); it++) {
	Command c(Command::ADD_IMAGE_URL, getInternalId());
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
