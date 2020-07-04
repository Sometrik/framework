#ifndef _WARNINGEVENT_H_
#define _WARNINGEVENT_H_

#include <Event.h>

#include <vector>

class WarningEvent : public Event {
 public:
  WarningEvent(const std::string & _code, const std::string & _warning)
    : code(_code), warning(_warning) { }

  void dispatch(EventHandler & handler) override {
    handler.handleWarningEvent(*this);
  }

  Event * dup() const override { return new WarningEvent(*this); }

  const std::string & getCode() const { return code; }
  const std::string & getWarning() const { return warning; }
  
 private:
  std::string code, warning;
};

#endif
