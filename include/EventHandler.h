#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

class EventBase;
class TouchEvent;
class CommandEvent;
class DrawEvent;
class PurchaseEvent;
class SysEvent;
class UpdateEvent;
class ResizeEvent;
class TimerEvent;
class TextEvent;
class OpenGLInitEvent;
class Command;
class LinearLayout;
class FWPlatform;

class EventHandler {
 public:
  EventHandler() { }
  virtual ~EventHandler() { }

  virtual void onEvent(EventBase & ev);
  virtual void onDrawEvent(DrawEvent & ev) { }
  virtual void onUpdateEvent(UpdateEvent & ev) { }
  virtual void onCommandEvent(CommandEvent & ev) { }
  virtual void onTouchEvent(TouchEvent & ev) { }
  virtual void onPurchaseEvent(PurchaseEvent & ev) { }
  virtual void onSysEvent(SysEvent & ev) { }
  virtual void onResizeEvent(ResizeEvent & ev) { }
  virtual void onTimerEvent(TimerEvent & ev) { }
  virtual void onTextEvent(TextEvent & ev) { }
  virtual void onOpenGLInitEvent(OpenGLInitEvent & ev) { }  
};

#endif
