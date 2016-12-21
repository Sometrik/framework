#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Command.h>

class FWApplication : public Element {
public:
  FWApplication(const char * _name, bool _full_screen = false)
    : name(_name), full_screen(_full_screen) { }
    
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_APPLICATION, getParentInternalId(), getInternalId());
    c.setTextValue(name);
    sendCommand(c);
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
  std::string name;
  bool full_screen;
};

#endif
