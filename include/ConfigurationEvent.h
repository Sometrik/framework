#ifndef _CONFIGURATIONEVENT_H_
#define _CONFIGURATIONEVENT_H_

#include <EventBase.h>

class ConfigurationEvent : public EventBase {
public:
  ConfigurationEvent(double _timestamp) : EventBase(_timestamp) { }
};

#endif
