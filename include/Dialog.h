#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <Command.h>
#include <FWPlatform.h>

class Dialog : public FWViewBase {
 public:
  Dialog(int _id = 0) : FWViewBase(_id) { }

  bool isA(const std::string & className) override {
    if (className == "Dialog") return true;
    return Element::isA(className);
  }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_DIALOG, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:

};

#endif
