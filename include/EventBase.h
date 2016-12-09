#ifndef _EVENTBASE_H_
#define _EVENTBASE_H_

#include <memory>

class Element;

class EventBase {
 public:
  EventBase(double _timestamp) : timestamp(_timestamp) { }
  virtual ~EventBase() { }

  virtual EventBase * dup() const = 0;
  virtual void dispatch(Element & element);
  
  Element * getSourceElement() { return source_element; }
  const Element * getSourceElement() const { return source_element; }

  double getTimestamp() const { return timestamp; }
  
  bool isHandled() const { return is_handled; }
  bool isRedrawNeeded() const { return redraw_needed; }

  void setIsHandled() { is_handled = true; }
  void requestRedraw() { redraw_needed = true; } 

 private:
  double timestamp;
  Element * source_element = 0;
  bool is_handled = false;
  bool redraw_needed = false;
};

#endif
