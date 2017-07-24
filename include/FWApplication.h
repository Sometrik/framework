#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Command.h>
#include <FWPlatform.h>

class FWApplication : public Element {
public:
  FWApplication(const char * _name, bool _full_screen = false)
    : name(_name), full_screen(_full_screen) { }

 FWApplication(const char * _name, const char * _iap_public_key, bool _full_screen = false)
   : name(_name), iap_public_key(_iap_public_key), full_screen(_full_screen) { }

  bool isA(const std::string & className) const override {
    if (className == "FWApplication") return true;
    return Element::isA(className);
  }

  void launchBrowser(const std::string & input_url) {
    sendCommand(Command(Command::LAUNCH_BROWSER, getInternalId(), input_url));
  }
  
  void postNotification(const std::string & title, const std::string & command) {
    sendCommand(Command(Command::POST_NOTIFICATION, getInternalId(), title, command));
  }

  Logger & getLogger() {
    if (!logger.get()) {
      logger = getPlatform().createLogger(name);
    }
    return *logger;
  }

  const std::string & getName() const { return name; }

  void setActiveViewId(int id) { activeViewId = id; }
  int getActiveViewId() const { return activeViewId; }

  void addToHistory(int view_internal_id) {
    view_back_history.push_back(view_internal_id);
    view_forward_history.clear();
  }

  int popViewBackHistory() {
    int id = 0;
    if (!view_back_history.empty()) {
      id = view_back_history.back();
      view_back_history.pop_back();
      view_forward_history.push_back(id);
    }
    return id;    
  }

  int popViewForwardHistory() {
    int id = 0;
    if (!view_forward_history.empty()) {
      id = view_forward_history.back();
      view_forward_history.pop_back();
      view_back_history.push_back(id);
    }
    return id;    
  }
  
 protected:
  bool isChildVisible(const Element & child) const override {
    return activeViewId == child.getInternalId();
  }
  void create() override {
    Command c(Command::CREATE_APPLICATION, getParentInternalId(), getInternalId());
    c.setFlags(iap_public_key.empty() ? 0 : 128);
    c.setTextValue(name);
    c.setTextValue2(iap_public_key);
    sendCommand(c);
  }

 private:
  std::string name, iap_public_key;
  bool full_screen;
  std::shared_ptr<Logger> logger;
  int activeViewId = 0;
  std::vector<int> view_back_history, view_forward_history;
};

#endif
