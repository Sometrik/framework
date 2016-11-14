#ifndef _TEXTEVENT_H_
#define _TEXTEVENT_H_

#include <EventBase.h>
#include <string>

class TextEvent : public EventBase {
public:
  TextEvent(double _timestamp, std::string _text, int _element_id = 0)
    : EventBase(_timestamp), element_id(_element_id), text(_text) { }

  void dispatch(Element & element) override;

  int getElementId() const { return element_id; }
  const std::string & getText() const { return text; }

 private:
  int element_id;
  std::string text;
};

#endif
