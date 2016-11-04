#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Message.h>

class FWApplication : public Element {
public:
  FWApplication(bool _full_screen = false) : full_screen(_full_screen) { }
    
  virtual bool loadEvents() { return false; }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendMessage(Message(Message::CREATE_APPLICATION, getParentInternalId(), getInternalId()));
  }

  void setCaption(const std::string & s) {
    Message m(Message::SET_CAPTION, getId());
    m.setTextValue(s);
    sendMessage(m);
  }

  void launchBrowser(const std::string & input_url) {
    sendMessage(Message(Message::LAUNCH_BROWSER, getInternalId(), input_url));
  }
  
  void postNotification(const std::string & title, const std::string & message) {
    sendMessage(Message(Message::POST_NOTIFICATION, getInternalId(), title, message));
  }

 private:
  bool full_screen;
};

#endif
