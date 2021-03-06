#ifndef _IMAGEEVENT_H_
#define _IMAGEEVENT_H_

#include <Event.h>
#include <PackedImageData.h>

#include <string>
#include <memory>

class ImageResponseEvent : public Event {
 public:
 ImageResponseEvent(bool _success, const std::string & _image_url, const std::shared_ptr<canvas::PackedImageData> & _image)
   : success(_success), image_url(_image_url), image(_image) { }
 ImageResponseEvent(bool _success, const std::string & _image_url)
   : success(_success), image_url(_image_url) { }

  const char * key() const override { return "image"; }
  Event * dup() const override { return new ImageResponseEvent(*this); }
  void dispatch(EventHandler & evh) override {
    evh.handleImageResponseEvent(*this);
    Event::dispatch(evh);
  }
  bool isBroadcast() const override { return true; }

  bool isSuccess() const { return success; }
  
  void setImage(const std::shared_ptr<canvas::PackedImageData> & _image) { image = _image; }

  void setIsLive(bool t) { is_live = t; }
  bool isLive() const { return is_live; }
  
  void cancel() { is_cancelled = true; }
  bool isCancelled() const { return is_cancelled; }
  
  const std::string & getImageUrl() const { return image_url; }
  std::shared_ptr<canvas::PackedImageData> & getImage() { return image; }

  bool hasImage() const { return image.get() != 0; }
  
 private:
  bool success;
  std::string image_url;
  std::shared_ptr<canvas::PackedImageData> image;
  bool is_live = false;
  bool is_cancelled = false;
};

#endif
