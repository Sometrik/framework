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
 FWActionSheet(const std::string & _title, int _id = 0) : Dialog(_title, _id) {
  }

  void addOption(int id, const std::string & name) {
    options.push_back(FWOption(id, name));
    auto & op = options.back();
    
    Command c(Command::ADD_OPTION, getInternalId());
    c.setValue(op.getId());
    c.setTextValue(op.getText());
    sendCommand(c);
  }

  const std::vector<FWOption> & getOptions() const { return options; }

  int showModal(Element * parent) override {
    if (!isInitialized()) {
      setParent(parent);
      initialize(&(parent->getThread()));
      initializeChildren();
    }
//    if (!isInitialized()) {
//      setParent(parent);
//      initialize(&(parent->getPlatform()));
//      initializeChildren();
//      load();
//    }
    return sendCommand(Command(Command::SHOW_ACTION_SHEET, getInternalId()));
//    Command c1(Command::SHOW_ACTION_SHEET, getInternalId());
//    sendCommand(c1);
//    return getPlatform().getModalResultValue();
  }

 protected:
  void create() override {
    Command c0(Command::CREATE_ACTION_SHEET, getParentInternalId(), getInternalId());
    c0.setTextValue(title);
    sendCommand(c0);
  }
  
 private:
  std::vector<FWOption> options;
};

#endif
