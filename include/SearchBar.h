#ifndef _SEARCHBAR_H_
#define _SEARCHBAR_H_

#include <Element.h>
#include <CommandEvent.h>
#include <Command.h>
#include <ValueEvent.h>

class SearchBar : public Element {
 public:
 SearchBar(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "SearchBar") return true;
    return Element::isA(className);
  }

  bool isVisible() const override {
    return getParent() && getParent()->isVisible();
  }

  void onValueEvent(ValueEvent & ev) override {
    value = ev.getTextValue();
    notify(value);
    CommandEvent ev2(getId(), ev.getValue(), ev.getValue2());
    ev2.dispatch(*this);
    ev.setHandled(true);
  }

  const std::string & getValue() const { return value; }

  void clear() override {
    value.clear();
    Element::clear();
  }

 protected:
  void create() override {
    Command c(Command::CREATE_SEARCHBAR, getParentInternalId(), getInternalId());
    sendCommand(c);
  }

 private:
  std::string value;
};

#endif
