#ifndef _IMAGEELEMENT_H_
#define _IMAGEELEMENT_H_

#include <Element.h>
#include <Command.h>
#include <ImageSet.h>
#include <PlatformThread.h>
#include <ImageRequestEvent.h>
#include <ImageResponseEvent.h>

class ImageElement : public Element {
 public:
  ImageElement(int _id = 0) : Element(_id) { }
  ImageElement(const ImageSet & _images, int _id = 0) : Element(_id), images(_images) { }
  ImageElement(const std::string & filename, int _id = 0) : Element(_id) {
    images.insert(filename);
  }

  bool isA(const std::string & className) const override {
    if (className == "ImageElement") return true;
    return Element::isA(className);
  }

  void handleImageRequestEvent(ImageRequestEvent & ev) {
    width = ev.getRequestedWidth();
    height = ev.getRequestedHeight();
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

#if 0
  std::unique_ptr<canvas::Context> createContext(unsigned int width, unsigned int height) {
    auto factory = getThread().createContextFactory();
    return factory->createContext(width, height);
  }
#endif

  const ImageSet & getImages() const { return images; }

  unsigned int getWidth() const { return width; }
  unsigned int getHeight() const { return height; }
  
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
  unsigned int width = 0, height = 0;
};

#endif
