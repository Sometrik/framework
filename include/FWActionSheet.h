#ifndef _FWACTIONSHEET_H_
#define _FWACTIONSHEET_H_

#include <Dialog.h>
#include <Command.h>

class FWActionSheet : public Dialog {
 public:
 FWActionSheet(const std::string & _title, int _id = 0) : Dialog(_title, _id) { }

  void addOption(int id, const std::string & name) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(id);
    c.setTextValue(name);
    sendCommand(c);
  }

 protected:
  void create() override {
    Command c0(Command::CREATE_ACTION_SHEET, getParentInternalId(), getInternalId());
    c0.setTextValue(getTitle());
    sendCommand(c0);
  }
};

#endif
