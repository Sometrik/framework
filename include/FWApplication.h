#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Command.h>

class FWApplication : public Element {
public:
  FWApplication(bool _full_screen = false) : full_screen(_full_screen) { }
    
  virtual bool loadEvents() { return false; }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    sendCommand(Command(Command::CREATE_APPLICATION, getParentInternalId(), getInternalId()));
  }

  void setCaption(const std::string & s) {
    Command c(Command::SET_CAPTION, getId());
    c.setTextValue(s);
    sendCommand(c);
  }

  void launchBrowser(const std::string & input_url) {
    sendCommand(Command(Command::LAUNCH_BROWSER, getInternalId(), input_url));
  }
  
  void postNotification(const std::string & title, const std::string & command) {
    sendCommand(Command(Command::POST_NOTIFICATION, getInternalId(), title, command));
  }

 private:
  bool full_screen;
};

#endif
