#ifndef _IMAGEUPLOADEVENT_H_
#define _IMAGEUPLOADEVENT_H_

#include <Event.h>

#include <EventHandler.h>
#include <string>
#include <cstring>

class ImageUploadEvent : public Event {
public:
  ImageUploadEvent(size_t _len, unsigned char * _image)
    : len(_len), image(_image) { }

  ImageUploadEvent(const ImageUploadEvent & other) {
    len = other.len;
    if (len) {
      image = new unsigned char[len];
      memcpy(image, other.image, len);
    } else {
      image = 0;
    }
  }

  ~ImageUploadEvent() {
    delete[] image;
  }

  ImageUploadEvent & operator=(const ImageUploadEvent & other) {
    delete[] image;
    len = other.len;
    if (len) {
      image = new unsigned char[len];
      memcpy(image, other.image, len);
    } else {
      image = 0;
    }
    return *this;
  }

  Event * dup() const override { return new ImageUploadEvent(*this); }
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onImageUploadEvent(*this);
      if (isHandled() && !handler) {
	handler = &element;
      }
    }
    Event::dispatch(element);
  }

  const unsigned char * getImage() const { return image; }
 private:
  size_t len;
  unsigned char * image;
};

#endif
