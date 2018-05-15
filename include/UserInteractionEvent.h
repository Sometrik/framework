#ifndef _USERINTERACTIONEVENT_H_
#define _USERINTERACTIONEVENT_H_

#include <Event.h>

#include <EventHandler.h>

class UserInteractionEvent : public Event {
 public:
  enum Type { STARTED = 1, ENDED };
 UserInteractionEvent(Type _type) : type(_type) { }

  const char * key() const override { return "userInteraction"; }
  Event * dup() const override { return new UserInteractionEvent(*this); }
  void dispatch(EventHandler & evh) override {
    if (!isHandled()) {
      evh.onUserInteractionEvent(*this);
      if (isHandled() && !handler) {
	handler = &evh;
      }
    }
    Event::dispatch(evh);
  }
  bool isBroadcast() const override { return true; }

  Type getType() const { return type; }

 private:
  Type type;
};

#endif
