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
    Message m0(Message::CREATE_ACTION_SHEET, getInternalId());
    m0.setTextValue(title);
    sendMessage(m0);
    for (auto & op : options) {
      Message m(Message::ADD_OPTION, getInternalId());
      m.setValue(op.getId());
      m.setTextValue(op.getText());
      sendMessage(m);
    }
    Message m1(Message::SHOW, getInternalId());
    sendMessage(m1);
    return 0;
  }
  
 private:
  std::string title;
  std::vector<FWOption> options;
};
#endif
