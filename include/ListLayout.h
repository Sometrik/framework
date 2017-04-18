#ifndef _LISTLAYOUT_H_
#define _LISTLAYOUT_H_

#include <UIElement.h>

#include <Command.h>
#include <CommandEvent.h>

class ListLayout : public Element {
 public:
  ListLayout() { }

  void
  addColumn(const char * title, int columnNumber) {
    Command c(Command::ADD_COLUMN, getInternalId());
    c.setValue(columnNumber);
    c.setTextValue(title);
    commands.push_back(c);
  }

  void addData(const char * text, int row, int column) {
    Command c(Command::SET_TEXT_DATA, getInternalId());
    c.setRow(row);
    c.setColumn(column);
    c.setTextValue(text);
    commands.push_back(c);
  }

  void addHeaderBar(int row, const char * text){
    Command c(Command::ADD_COLUMN, getInternalId(), text);
    c.setValue(row);
    commands.push_back(c);
  }

 protected: 
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_LISTVIEW, getParentInternalId(), getInternalId());
    c.setLayoutWeight(getLayoutWeight());
    sendCommand(c);

    for (int i = 0; i < commands.size(); i++){
      sendCommand(commands.at(i));
    }

  }

 private:
  std::vector<Command> commands;
};

#endif
