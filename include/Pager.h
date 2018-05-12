#ifndef _PAGER_H_
#define _PAGER_H_

#include <Element.h>
#include <Command.h>
#include <CommandEvent.h>
#include <ValueEvent.h>

class Pager : public Element {
 public:
  Pager(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "Pager") return true;
    return Element::isA(className);
  }

  void onValueEvent(ValueEvent & ev) override {
    visible_view_index = ev.getValue();
    if (visible_view_index >= 0 && visible_view_index < getChildren().size()) {
      getChildren()[visible_view_index]->refresh();
    }

    notify();
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  void setVisibleView(int position) {
    visible_view_index = position;
    if (position >= 0 && position < getChildren().size()) {
      getChildren()[position]->refresh();
    }
    Command c(Command::SET_INT_VALUE, getInternalId());
    c.setValue(position);
    sendCommand(c);
  }

  bool isChildVisible(const Element & child) const override {
    if (!isVisible()) {
      return false;
    } else if (visible_view_index >= 0 && visible_view_index < getChildren().size()) {
      return getChildren()[visible_view_index]->getInternalId() == child.getInternalId();
    } else {
      return false;
    }
  }

 protected:
  void create() override {
    Command c(Command::CREATE_PAGER, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
  int visible_view_index = 0;
};

#endif
