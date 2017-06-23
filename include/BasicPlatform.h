#ifndef _BASICPLATFORM_H_
#define _BASICPLATFORM_H_

#include <FWPlatform.h>
#include <CurlClient.h>

#ifndef NO_CANVAS
#include <Context.h>
#endif

class BasicPlatform : public FWPlatform {
 public:
 BasicPlatform() : FWPlatform(1.0f) { }

  std::string getBundleFilename(const char * filename) override {
    return filename;
  }
  std::string getLocalFilename(const char * filename, FileType type) override {
    return filename;
  }
#ifndef NO_CANVAS
  std::unique_ptr<canvas::ContextFactory> createContextFactory() const override {
    return std::unique_ptr<canvas::ContextFactory>(new canvas::NullContextFactory);
  }
#endif
  std::unique_ptr<HTTPClientFactory> createHTTPClientFactory() const override {
    return std::unique_ptr<HTTPClientFactory>(new CurlClientFactory);
  }
  void pushEvent(const Event & ev) override {

  }
  void sendCommand2(const Command & command) {
    
  }
  std::string loadTextAsset(const char * filename) {
    return "";
  }
};

#endif
