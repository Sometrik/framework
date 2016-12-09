#ifndef _EVENT_H_
#define _EVENT_H_

class EventHandler;

class Event {
 public:
  Event(double _timestamp) : timestamp(_timestamp) { }
  virtual ~Event() { }

  virtual Event * dup() const = 0;
  virtual void dispatch(EventHandler & ev);
  
  double getTimestamp() const { return timestamp; }
  
  bool isHandled() const { return is_handled; }
  bool isRedrawNeeded() const { return redraw_needed; }

  bool needAnimation() const { return animation_needed; }
  bool needUpdateLayout() const { return update_layout_needed; }

  void setHandled() { is_handled = true; }
  void requestRedraw() { redraw_needed = true; }
  void requestAnimation() { request_animation = true; }
  void updateLayout() { update_layout_needed = true; }

  void update() { redraw_needed = true; }

 private:
  double timestamp;
  bool is_handled = false;
  bool redraw_needed = false;
  bool animation_needed = false;
  bool update_layout_needed = false;
};

#endif
