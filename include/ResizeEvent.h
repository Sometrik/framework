#ifndef _RESIZEEVENT_H_
#define _RESIZEEVENT_H_

#include <EventBase.h>

class ResizeEvent : public EventBase {
public:
 ResizeEvent(double _timestamp, int _logical_width, int _logical_height, int _actual_width, int _actual_height )
   : EventBase(_timestamp), logical_width(_logical_width), logical_height(_logical_height), actual_width(_actual_width), actual_height(_actual_height) { }

  void dispatch(Element & element) override;

  int getLogicalWidth() const { return logical_width; }
  int getLogicalHeight() const { return logical_height; }
  int getActualWidth() const { return actual_width; }
  int getActualHeight() const { return actual_height; }
   
 private:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
};

#endif
