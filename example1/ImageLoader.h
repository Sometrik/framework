#ifndef _IMAGELOADER_H_
#define _IMAGELOADER_H_

#include <Runnable.h>
#include <ImageRequestEvent.h>
#include <InternalFormat.h>

namespace canvas {
  class PackedImageData;
};

#include <string>
#include <memory>

class ImageLoader : public Runnable {
 public:
  ImageLoader(const ImageRequestEvent & _ev, const std::string & _user_agent);
  
  void run();
  bool handleImageData(const std::string & data, bool is_live);

 protected:
  std::unique_ptr<canvas::PackedImageData> loadImageFromMemory(const unsigned char * buffer, size_t size, canvas::InternalFormat image_format, unsigned int required_width, unsigned int required_height, unsigned int required_levels) const;

 private:
  ImageRequestEvent request;
  std::string user_agent;
};

#endif
