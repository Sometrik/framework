#ifndef _IMAGEREQUESTEVENT_H_
#define _IMAGEREQUESTEVENT_H_

#include <Event.h>
#include <EventHandler.h>

#include <string>
// #include <skey.h>

#include <InternalFormat.h>

class ImageRequestEvent : public Event {
 public:
  enum RequestMode {
    REQUEST = 1,
    CANCEL
  };
  enum RequestPriority {
    REALTIME = 1,
    NORMAL,
    LOW
  };
 ImageRequestEvent(RequestMode _mode, int _source_internal_id) : mode(_mode), priority(NORMAL), source_internal_id(_source_internal_id), requested_width(0), requested_height(0), requested_levels(0) { }

   ImageRequestEvent(RequestMode _mode, int _source_internal_id, const std::string & _image_url, RequestPriority _priority, unsigned int _requested_width, unsigned int _requested_height, unsigned int _requested_levels = 1) : mode(_mode), image_url(_image_url), priority(_priority), source_internal_id(_source_internal_id), requested_width(_requested_width), requested_height(_requested_height), requested_levels(_requested_levels) { }

  const char * key() const override { return "imageRequest"; }
  Event * dup() const override { return new ImageRequestEvent(*this); }
  bool isBroadcast() const override { return true; }

  void dispatch(EventHandler & evh) override {
    evh.handleImageRequestEvent(*this);
    Event::dispatch(evh);
  }

  RequestMode getRequestMode() const { return mode; }
  
  const std::string & getImageUrl() const { return image_url; }
  unsigned int getRequestedWidth() const { return requested_width; }
  unsigned int getRequestedHeight() const { return requested_height; }
  unsigned int getRequestedLevels() const { return requested_levels; }

  RequestPriority getPriority() const { return priority; }
  void setSubPriority(int p) { sub_priority = p; }
  int getSubPriority() const { return sub_priority; }
 
  //  void setTargetKey(const skey & _target_key) { target_key = _target_key; }
  // const skey & getTargetKey() const { return target_key; }
  
  void setInternalFormat(canvas::InternalFormat _format) { format = _format; }
  canvas::InternalFormat getInternalFormat() const { return format; }

  int getSourceInternalId() const { return source_internal_id; }
  
 private:
  RequestMode mode;
  std::string image_url;
  RequestPriority priority;
  unsigned int requested_width, requested_height, requested_levels;
  int sub_priority = 0;
  // skey target_key;
  canvas::InternalFormat format = canvas::NO_FORMAT;
  int source_internal_id;
};

#endif
