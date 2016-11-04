#ifndef _EVENTBASE_H_
#define _EVENTBASE_H_

class Element;

class EventBase {
 public:
  EventBase() { }
  virtual ~EventBase() { }

  virtual void dispatch(Element & element) = 0;
  
  Element * getSourceElement() { return source_element; }
  const Element * getSourceElement() const { return source_element; }
  
 private:
  Element * source_element = 0;
};

#endif
