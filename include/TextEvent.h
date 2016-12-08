#ifndef _TEXTEVENT_H_
#define _TEXTEVENT_H_

#include <EventBase.h>
#include <string>

class TextEvent : public EventBase {
public:
  TextEvent(double _timestamp, std::string _text)
    : EventBase(_timestamp), text(_text) { }

  std::shared_ptr<EventBase> dup() const override { return std::make_shared<TextEvent>(*this); }
  void dispatch(Element & element) override;

  const std::string & getText() const { return text; }

 private:
  std::string text;
};

#endif
