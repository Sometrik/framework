#ifndef _FWCONTEXTBASE_H_
#define _FWCONTEXTBASE_H_

#include <Element.h>

class FWContextBase : public Element {
public:
  FWContextBase() { }
    
  bool createWindow(const char * title, int requested_width, int requested_height);
  virtual bool loadEvents() { return false; }
};

#endif
