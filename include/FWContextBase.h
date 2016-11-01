#ifndef _FWCONTEXTBASE_H_
#define _FWCONTEXTBASE_H_

#include <Element.h>
#include <Message.h>

class FWContextBase : public Element {
public:
  FWContextBase() { }
    
  bool createWindow(const char * title, int requested_width, int requested_height);
  virtual bool loadEvents() { return false; }
  
  void setCaption(const std::string & s) {
    Message m(Message::SET_CAPTION);
    m.setTextValue(s);
    sendMessage(ev);
  }
};

#endif
