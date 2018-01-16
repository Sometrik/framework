#ifndef _IMAGEUPLOADEVENT_H_
#define _IMAGEUPLOADEVENT_H_

#include <Event.h>

#include <EventHandler.h>
#include <string>

class ImageUploadEvent : public Event {
public:
  ImageUploadEvent(const unsigned char * _image)
    : image(_image) { }

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
  const unsigned char * image;
};

#endif
