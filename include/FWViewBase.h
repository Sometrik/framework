#ifndef _FWVIEW_H_
#define _FWVIEW_H_

class FWViewBase {
 public:
  
  FWViewBase(){

  }
  virtual ~FWViewBase() { }

  bool createWindow(const char * title, int requested_width, int requested_height);

    virtual void onDraw() = 0;
    virtual void onCmdLine(int argc, char *argv[]) { }

    virtual bool onKeyPress(char c, double timestamp, int x, int y) { return false; }
    virtual bool onUpdate(double timestamp) { return false; }
    virtual bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) {
      logical_width = _logical_width;
      logical_height = _logical_height;
      actual_width = _actual_width;
      actual_height = _actual_height;
      return false;
    }

    virtual bool touchesBegin(int x, int y, double timestamp, long identifier) { return false; }
    virtual bool touchesEnded(int x, int y, double timestamp, long identifier) { return false; }
    virtual bool touchesMoved(int x, int y, double timestamp, long identifier) { return false; }
    virtual bool flushTouches(int mode, double timestamp) { return false; }
    virtual bool onShake(double timestamp) { return false; }

    virtual bool loadEvents() { return false; }

    int getLogicalWidth() const { return logical_width; }
    int getLogicalHeight() const { return logical_height; }
    int getActualWidth() const { return actual_width; }
    int getActualHeight() const { return actual_height; }

    void setWindowSize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) {
      logical_width = _logical_width;
      logical_height = _logical_height;
      actual_width = _actual_width;
      actual_height = _actual_height;
    }

    virtual void showView() = 0;

protected:
  int logical_width = 0, logical_height = 0, actual_width = 0, actual_height = 0;
};

#endif
