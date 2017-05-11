#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <Notifier.h>

class Event;
class TouchEvent;
class CommandEvent;
class DrawEvent;
class PurchaseEvent;
class SysEvent;
class UpdateEvent;
class ResizeEvent;
class TimerEvent;
class ValueEvent;
class OpenGLInitEvent;

class WindowEvent;
class UICommandEvent;
class MouseEvent;
class PaintEvent;
class PaintEventCanvas;
class AnimationEvent;
class LayoutEvent;
class SelectionEvent;
class ScrollEvent;
class ContextMenuEvent;
class ImageRequestEvent;
class ImageEvent;
class PositionEvent;
class GraphChangeEvent;
class RebuildEvent;
class LicenseEvent;
class EdgeEvent;
class PrepareEvent;
class ThreadEvent;
class VisiblePostsEvent;
class LeaveWorkspaceEvent;
class MultiTouchEvent;
class LoadEvent;
class SearchEvent;

class EventHandler : public Notifier {
 public:
  bool call() override { return call(true); }
  bool call(bool t) override { return call(t ? 1 : 0); }
  bool call(int i) override { return true; }
  bool call(const std::string & s) override { return true; }

  virtual void onEvent(Event & ev) { }
  virtual void onDrawEvent(DrawEvent & ev) { }
  virtual void onUpdateEvent(UpdateEvent & ev) { }
  virtual void onCommandEvent(CommandEvent & ev) { }
  virtual void onTouchEvent(TouchEvent & ev) { }
  virtual void onScrollEvent(ScrollEvent & ev) { }
  virtual void onPurchaseEvent(PurchaseEvent & ev) { }
  virtual void onSysEvent(SysEvent & ev) { }
  virtual void onResizeEvent(ResizeEvent & ev) { }
  virtual void onTimerEvent(TimerEvent & ev) { }
  virtual void onValueEvent(ValueEvent & ev) { }
  virtual void onOpenGLInitEvent(OpenGLInitEvent & ev) { }

  virtual void handleWindowEvent(WindowEvent & ev) { }
  virtual void handleUICommandEvent(UICommandEvent & ev) { }
  virtual void handlePrepareEvent(PrepareEvent & ev) { }
  virtual void handlePaintEventRecursive(PaintEvent & ev) { }
  virtual void handlePaintEvent(PaintEvent & ev) { }
  virtual void handlePaintEventCanvas(PaintEventCanvas & ev) { }
  virtual void handleSelectionEvent(SelectionEvent & ev) { }
  virtual void handleAnimationEvent(AnimationEvent & ev) { }
  virtual void handleLayoutEvent(LayoutEvent & ev) { }
  virtual void handleGraphChangeEvent(GraphChangeEvent & ev) { }
  virtual void handleRebuildEvent(RebuildEvent & ev) { }
  virtual void handleLicenseEvent(LicenseEvent & ev) { }
  virtual void handleEdgeEvent(EdgeEvent & ev) { }
  virtual void handleThreadEvent(ThreadEvent & ev) { }
  
  virtual void handleContextMenuEvent(ContextMenuEvent & ev) { }
  virtual void handleMouseDownEvent(MouseEvent & ev) { }
  virtual void handleMouseUpEvent(MouseEvent & ev) { }
  virtual void handleMouseMotionEvent(MouseEvent & ev) { }
  virtual void handleMouseDoubleClickEvent(MouseEvent & ev) { }
  virtual void handleMouseClickEvent(MouseEvent & ev) { }
  virtual void handleMouseDragEvent(MouseEvent & ev) { }
  virtual void handleMouseScrollEvent(MouseEvent & ev) { }
  virtual void handleMouseEvent(MouseEvent & ev) { }
  virtual void handleImageRequestEvent(ImageRequestEvent & ev) { }
  virtual void handleImageEvent(ImageEvent & ev) { }
  virtual void handleVisiblePostsEvent(VisiblePostsEvent & ev) { }
  virtual void handleLeaveWorkspaceEvent(LeaveWorkspaceEvent & ev) { }
  virtual void handleMultiTouchEvent(MultiTouchEvent & ev) { }
  virtual void handleLoadEvent(LoadEvent & ev) { }
  virtual void handleSearchEvent(SearchEvent & ev) { }
  
  virtual bool acceptPositionEvent(const PositionEvent & ev, float margin = 0) const { return false; }
  virtual bool isVisibleOnWorkspace(int w) const { return true; }  
};

#endif
