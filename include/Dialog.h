#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Command.h>
#include <FWPlatform.h>
#include <FWViewBase.h>
#include <ValueEvent.h>
#include <CommandEvent.h>

class Dialog : public FWViewBase {
 public:
  Dialog(int _id = 0) : FWViewBase(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "Dialog") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    notify();
    CommandEvent ev2(ev.getTimestamp(), getId());
    ev2.dispatch(*this);
  }

protected:
  void create() override {
    Command c(Command::CREATE_DIALOG, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
  }
 private:

};

#endif
