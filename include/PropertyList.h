#ifndef _PROPERTYLIST_H_
#define _PROPERTYLIST_H_

#include <Property.h>

class PropertyList {
 public:
  PropertyList() { }

 private:
  std::map<std::string, std::shared_ptr<PropertyBase> > data;
};

#endif
