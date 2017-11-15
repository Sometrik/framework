#ifndef _FWACTIONSHEET_H_
#define _FWACTIONSHEET_H_

#include <Dialog.h>
#include <Command.h>

class FWOption {
 public:
 FWOption(int _id, const std::string & _text) : id(_id), text(_text) { }

  int getId() const { return id; }
  const std::string & getText() const { return text; }
  
 private:
  int id;
  std::string text;
};

class FWActionSheet : public Dialog {
 public:
 FWActionSheet(const std::string & _title, int _id = 0) : Dialog(_title, _id) { }

  void addOption(int id, const std::string & name) {
    options.push_back(FWOption(id, name));
    auto & op = options.back();
    
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(op.getId());
    c.setTextValue(op.getText());
    sendCommand(c);
  }

  const std::vector<FWOption> & getOptions() const { return options; }

 protected:
  void create() override {
    Command c0(Command::CREATE_ACTION_SHEET, getParentInternalId(), getInternalId());
    sendCommand(c0);
  }
  
 private:
  std::vector<FWOption> options;
};

#endif
