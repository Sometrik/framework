#include <SysEvent.h>
#include <Element.h>

void
SysEvent::dispatch(Element & element) {
  element.onSysEvent(*this);

  if (!isHandled()){
    for (auto & c: element.getChildren()){
      dispatch(*c);
    }
  }
  //Do not call super class.
}
