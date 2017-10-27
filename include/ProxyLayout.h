#ifndef _PROXYLAYOUT_H_
#define _PROXYLAYOUT_H_

#include <Element.h>
class ProxyLayout : public Element {
public:
  ProxyLayout(int _id) : Element(_id) { }
  
  void onProxyEvent(ProxyEvent & ev) {
    switch (ev.getType()) {
    case ProxyEvent::REQUEST_CONTENT: {
      auto e = createContent(ev.getKey());
      addChild(e);
    }
      break;
    case ProxyEvent::DELETE_CONTENT: {
      auto it = content.find(ev.getKey());
      if (it != content.end()) {
        content.remove(it);
      }
    }
      break;
    }
  }
  
  void clear() {
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }
  
  void addProxy(const std::string & key) {
    Command c(Command::ADD_OPTION, getInternalId());
    c.setTextValue(key);
    sendCommand(c);
  }
 
protected:
  virtual std::shared_ptr<Element> createContent(const std::string & key) = 0;
  
  std::shared_ptr<Element> getContent(const std::string & key) {
    auto it = content.find(key);
    if (it != content.end()) {
      return it->second;
    } else {
      return std::shared_ptr<Element>(0);
    }
  }  
  
private:
  std::unordered_map<std::string, std::shared_ptr<Element> > content;
};

#endif
