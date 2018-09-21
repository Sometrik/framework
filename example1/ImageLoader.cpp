#include "ImageLoader.h"

#include <PlatformThread.h>
#include <ImageResponseEvent.h>
#include <StringUtils.h>

#include <URI.h>
#include <HTTPClient.h>
#include <Context.h>

#include <iostream>

using namespace std;

class ImageInterface : public HTTPClientInterface {
 public:
  ImageInterface(ImageLoader * _loader) : loader(_loader) { }

  void handleHeader(const std::string & key, const std::string & value) override {
    // multipart/x-mixed-replace; boundary=myboundary
    if (key == "Content-Type" && value.compare(0, 25, "multipart/x-mixed-replace") == 0) {
      string::size_type pos = value.find("boundary=");
      assert(pos != string::npos);
      boundary = value.substr(pos + 9);
      if (boundary.compare(0, 2, "--") != 0) boundary = "--" + boundary;
      multipart_mode = true;
      cerr << "got multipart mode, boundary = " << boundary << "\n";      
    }
  }
  
  bool handleChunk(size_t len, const char * chunk) override {
    buffer += string(chunk, len);
    if (multipart_mode) {
      while ( 1 ) {      
	string::size_type pos = buffer.find(boundary);
	if (pos != string::npos) {
	  string part = buffer.substr(0, pos);
	  buffer.erase(0, pos + boundary.size());      
	  bool r = processData(part);
	  if (!r) return false;
	} else {
	  break;
	}
      }
    }
    return true;
  }
  void handleDisconnect() override {
    if (!multipart_mode) {
      loader->handleImageData(buffer, false);
    }
  }
  bool processData(const std::string & chunk) {
    // cerr << "processing part, size = " << chunk.size() << endl;
    if (chunk.size() >= 4) {
      string::size_type pos = chunk.find("\x0d\x0a\x0d\x0a");
      assert(pos != string::npos);
      string real_data = chunk.substr(pos + 4);
      return loader->handleImageData(real_data, true);
    }
    return true;
  }
  const std::string & getBuffer() const { return buffer; }
  bool isMultipart() const { return multipart_mode; }
  
 protected:  
  
 private:
  ImageLoader * loader;
  string buffer;
  bool multipart_mode = false;
  string boundary;
};

ImageLoader::ImageLoader(const ImageRequestEvent & _ev, const std::string & _user_agent) 
  : request(_ev),
    user_agent(_user_agent)
{
}

std::unique_ptr<canvas::PackedImageData>
ImageLoader::loadImageFromMemory(const unsigned char * buffer, size_t size, canvas::InternalFormat image_format, unsigned int required_width, unsigned int required_height, unsigned int required_levels) const {
  auto t = getThreadPtr();
  auto factory = t->createContextFactory();
  auto img = factory->createImage();
  img->decode(buffer, size);
  if (required_width && required_height) {
    auto & data = img->getData();
    if (data.getWidth() != required_width || data.getHeight() != required_height) {
      img->scale(required_width, required_height);
    }
  }
  return img->pack(image_format != 0 ? image_format : canvas::RGBA8, 1);
}

bool
ImageLoader::handleImageData(const std::string & data, bool is_live) {
  // cerr << "ImageLoader: processing data, size = " << data.size() << "\n";
  std::shared_ptr<canvas::PackedImageData> image = loadImageFromMemory((const unsigned char *)data.data(), data.size(), request.getInternalFormat(), request.getRequestedWidth(), request.getRequestedHeight(), request.getRequestedLevels());
  auto t = getThreadPtr();
  
  ImageResponseEvent ev(true, request.getImageUrl(), image);
  t->postEvent(request.getSourceInternalId(), ev);

  return !t->testDestroy() && !ev.isCancelled();
}

void
ImageLoader::run() {
  auto t = getThreadPtr();
  auto factory = t->createHTTPClientFactory();
  auto http = factory->createClient(user_agent, true, true);
  
  try {
    cerr << "ImageLoader: loading image " << request.getImageUrl() << endl;

    ImageInterface ii(this);
    http->Get(request.getImageUrl(), ii, true);
  } catch (exception & e) {
    cerr << "got exception: " << e.what() << endl;
  }
}
