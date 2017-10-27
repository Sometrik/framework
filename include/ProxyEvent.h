#ifndef _PROXYEVENT_H_
#define _PROXYEVENT_H_

#include <Event.h>

#include <string>

class ProxyEvent : public Event {
public:
  enum Type { REQUEST_CONTENT = 1, DELETE_CONTENT }
  
  ProxyEvent(Type _type, const std::string & _key) : type(_type), key(_key) { }
  
  Event * dup() const override { return new ProxyEvent(*this); }
  
  void dispatch(EventHandler & element) override {
    if (!isHandled()) {
      element.onProxyEvent(*this);
      if (isHandled() && !handler) {
        handler = &element;
      }
    }
    Event::dispatch(element);
  }
  
  Type getType() const { return type; }
  const std::string & getKey() const { return key; }
  
private:
  Type type;
  std::string key;
};

#endif
