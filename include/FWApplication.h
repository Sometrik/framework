#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Command.h>
#include <PlatformThread.h>
#include <FWPreferences.h>
#include <MobileAccount.h>
#include <SysEvent.h>

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
  
  Logger & getLogger() {
    if (!logger.get()) {
      logger = getThread().createLogger(name);
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

  virtual int popViewBackHistory() {
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
  
  void onSysEvent(SysEvent & ev) {
    if (ev.getType() == SysEvent::BACK) {
      int poppedView = popViewBackHistory();
      if (poppedView != 0) {
        Command c(Command::SET_INT_VALUE, poppedView);
        c.setValue(2);
        sendCommand(c);
      } else {
        Command c(Command::QUIT_APP, poppedView);
        sendCommand(c);
      }
    }
  }

  void setMobileAccount(const MobileAccount & account) { mobileAccount = account; }
  const MobileAccount getMobileAccount() const { return mobileAccount; }

  void setPreferences(const FWPreferences & _preferences) { preferences = _preferences; }
  FWPreferences & getPreferences() { return preferences; }
  const FWPreferences & getPreferences() const { return preferences; }
  
 protected:
  bool isChildVisible(const Element & child) const override {
    if (!isVisible()) return false;
    else return activeViewId == child.getInternalId();
  }

  void create() override {
    Command c(Command::CREATE_APPLICATION, getParentInternalId(), getInternalId());
    c.setFlags(iap_public_key.empty() ? 0 : 128);
    c.setTextValue(name);
    c.setTextValue2(iap_public_key);
    sendCommand(c);
  }

  void savePreferences() {
    for (auto & row : preferences.getChanges()) {
      Command c(Command::UPDATE_PREFERENCE, getInternalId());
      c.setTextValue(row.first);
      c.setTextValue2(row.second);
      sendCommand(c);
    }
    sendCommand(Command(Command::COMMIT_PREFERENCES, getInternalId()));
    preferences.clearChanges();
  }

 private:
  std::string name, iap_public_key;
  bool full_screen;
  std::shared_ptr<Logger> logger;
  int activeViewId = 0;
  std::vector<int> view_back_history, view_forward_history;
  FWPreferences preferences;
  MobileAccount mobileAccount;
};

#endif
