#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Runnable.h>
#include <Command.h>
#include <FWPreferences.h>
#include <MobileAccount.h>

class FWApplication : public Runnable {
public:
  FWApplication(const char * _name, bool _full_screen = false)
    : name(_name), full_screen(_full_screen) { }

 FWApplication(const char * _name, const char * _iap_public_key, bool _full_screen = false)
   : name(_name), iap_public_key(_iap_public_key), full_screen(_full_screen) { }

  bool isA(const std::string & className) const override {
    if (className == "FWApplication") return true;
    return Runnable::isA(className);
  }
  
  std::string getName() const override { return name; }

  virtual void setActiveViewId(int view_id) { activeViewId = view_id; }
  int getActiveViewId() const { return activeViewId; }

  void addToHistory(int view_internal_id) {
    view_back_history.push_back(view_internal_id);
  }

  int getViewBackHistory() {
    if (!view_back_history.empty()) {
      return view_back_history.back();
    } else {
      return 0;
    }
  }

  int popViewBackHistory() {
    int view_id = 0;
    if (!view_back_history.empty()) {
      view_id = view_back_history.back();
      view_back_history.pop_back();
    }
    return view_id;
  }
  
  void onSysEvent(SysEvent & ev) override;

  void setMobileAccount(const MobileAccount & account) { mobileAccount = account; }
  const MobileAccount getMobileAccount() const { return mobileAccount; }

  void setPreferences(const FWPreferences & _preferences) { preferences = _preferences; }
  FWPreferences & getPreferences() { return preferences; }
  const FWPreferences & getPreferences() const { return preferences; }
  void shareLink(std::string url) {
    Command c(Command::SHARE_LINK, getInternalId());
    c.setTextValue(url);
    sendCommand(c);
  }
  void setVersion(const std::string & v) { versionText = v; }
  void setBuildNumber(const std::string & n) { buildNumber = n; }

  void showMessageDialog(const std::string & title, const std::string & message);
  std::string showInputDialog(const std::string & title, const std::string & message);

  void selectFromGallery() {
    Command c(Command::SELECT_FROM_GALLERY, getInternalId());
    sendCommand(c);
  }

  bool isDebugModeEnabled() const { return debug_mode_enabled; }
  void setDebugModeEnabled(bool t) { debug_mode_enabled = t; }

 protected:
  void run() override;

  void create() override {
    Command c(Command::CREATE_APPLICATION, getParentInternalId(), getInternalId());
    c.setFlags(iap_public_key.empty() ? 0 : 128);
    c.setTextValue(name);
    c.setTextValue2(iap_public_key);
    sendCommand(c);
  }

  bool isChildVisible(const Element & child) const override {
    return activeViewId == child.getInternalId();
  }

  void savePreferences() {
    for (auto & row : preferences.getDeletedKeys()) {
      Command c(Command::DELETE_PREFERENCE, getInternalId());
      c.setTextValue(row);
      sendCommand(c);
    }
    for (auto & row : preferences.getChanges()) {
      Command c(Command::UPDATE_PREFERENCE, getInternalId());
      c.setTextValue(row.first);
      c.setTextValue2(row.second);
      sendCommand(c);
    }
    sendCommand(Command(Command::COMMIT_PREFERENCES, getInternalId()));
    preferences.clearChanges();
  }

  const std::string & getVersion() const { return versionText; }
  const std::string & getBuildNumber() const { return buildNumber; }

 private:
  std::string name, iap_public_key;
  bool full_screen;
  int activeViewId = 0;
  std::vector<int> view_back_history;
  FWPreferences preferences;
  MobileAccount mobileAccount;
  std::string versionText;
  std::string buildNumber;
  bool debug_mode_enabled = false;
};

#endif
