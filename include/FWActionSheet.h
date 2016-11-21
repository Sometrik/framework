#ifndef _FWACTIONSHEET_H_
#define _FWACTIONSHEET_H_

#include <Element.h>

class FWOption {
 public:
 FWOption(int _id, const std::string & _text) : id(_id), text(_text) { }

  int getId() const { return id; }
  const std::string & getText() const { return text; }
  
 private:
  int id;
  std::string text;
};

class FWActionSheet : public Element {
 public:
  FWActionSheet(Element & parent, const std::string & _title)
    : title(_title) {
    FWPlatform & p = parent.getPlatform();
    initialize(&p);
  }

  void addOption(int id, const std::string & name) {
    options.push_back(FWOption(id, name));
  }

  const std::string & getTitle() const { return title; }
  const std::vector<FWOption> & getOptions() const { return options; }

  int showModal() override {
    Command c0(Command::CREATE_ACTION_SHEET, getInternalId());
    c0.setTextValue(title);
    sendMessage(c0);
    for (auto & op : options) {
      Command c(Command::ADD_OPTION, getInternalId());
      c.setValue(op.getId());
      c.setTextValue(op.getText());
      sendMessage(c);
    }
    Command c1(Command::SHOW, getInternalId());
    sendMessage(c1);
    return 0;
  }
  
 private:
  std::string title;
  std::vector<FWOption> options;
};
#endif
