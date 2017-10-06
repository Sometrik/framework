#ifndef _FWPLATFORM_H_
#define _FWPLATFORM_H_

#include <Event.h>
#include <Mutex.h>
#include <Element.h>

#include <unordered_map>

class FWPlatform {
 public:  
  FWPlatform() { }
  
  void postEvent(int internal_id, Event & ev) {
    auto e = getRegisteredElement(internal_id);
    if (e) {
      ev.dispatch(*e);
    }
  }
  
  void registerElement(Element * e) {
    MutexLocker m(mutex);
    registered_elements[e->getInternalId()] = e;
  }

  void unregisterElement(Element * e) {
    MutexLocker m(mutex);
    registered_elements.erase(e->getInternalId());
  }

 protected:
  Element * getRegisteredElement(int internal_id) {
    MutexLocker m(mutex);
    auto it = registered_elements.find(internal_id);
    return it != registered_elements.end() ? it->second : 0;
  }
    
 private:
  std::unordered_map<int, Element *> registered_elements;
  Mutex mutex;
};

#endif
