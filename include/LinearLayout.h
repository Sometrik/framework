#ifndef _LINEARLAYOUT_H_
#define _LINEARLAYOUT_H_

#include <Element.h>
#include <Command.h>

#define FW_VERTICAL	1
#define FW_HORIZONTAL	2

class LinearLayout : public Element {
 public:
  LinearLayout(int _direction, int _id) : Element(_id), direction(_direction) { }

  void
  setGravity(const char * gravity){
    Command c(Command::SET_STYLE, getInternalId());
    c.setTextValue("gravity");
    if (gravity == "bottom"){
      c.setTextValue2("bottom");
    } else if (gravity == "top"){
      c.setTextValue2("top");
    } else if (gravity == "left"){
      c.setTextValue2("left");
    } else if (gravity == "right"){
      c.setTextValue2("right");
    }
    additionalInitCommands.push_back(c);
  }

  bool isA(const std::string & className) override {
    if (className == "LinearLayout") return true;
    return Element::isA(className);
  }

 protected:
  void initialize(FWPlatform * _platform) override {
    Element::initialize(_platform);
    Command c(Command::CREATE_LINEAR_LAYOUT, getParentInternalId(), getInternalId());
    c.setValue(direction);
    sendCommand(c);

    for (int i = 0; i < additionalInitCommands.size(); i++){
      sendCommand(additionalInitCommands.at(i));
    }
  }
			       
 private:
  int direction = FW_VERTICAL;
  std::vector<Command> additionalInitCommands;

};

#endif
