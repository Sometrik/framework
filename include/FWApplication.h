#ifndef _FWAPPLICATION_H_
#define _FWAPPLICATION_H_

#include <Element.h>
#include <Message.h>

class FWApplication : public Element {
public:
  FWApplication(bool _full_screen = false) : full_screen(_full_screen) { }
    
  virtual bool loadEvents() { return false; }
  
  void setCaption(const std::string & s) {
    Message m(Message::SET_CAPTION);
    m.setTextValue(s);
    sendMessage(m);
  }

 private:
  bool full_screen;
};

#endif
