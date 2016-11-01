#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <memory>
#include <vector>

class TouchEvent;
class FWPlatformBase;
class Message;

class Element {
 public:
  Element() {
  }

  virtual ~Element() { }


  virtual void initialize(FWPlatformBase * _platform);
      virtual void onDraw() { }
      virtual void onCmdLine(int argc, char *argv[]) { }

      virtual bool onKeyPress(char c, double timestamp, int x, int y) { return false; }
      virtual bool onUpdate(double timestamp) { return false; }
      virtual bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) { return false; }

      virtual bool onTouchEvent(TouchEvent & touchEvent) { return false; }
      virtual bool flushTouches(int mode, double timestamp) { return false; }
      virtual bool onShake(double timestamp) { return false; }

      virtual bool loadEvents() { return false; }
      void addChild(std::shared_ptr<Element> & element){
        element->initialize(platform);
        children.push_back(element);
      }

      int getId(){ return id; }

      void sendMessage(const Message & message);

      FWPlatformBase & getPlatform() { return *platform; }

protected:
  FWPlatformBase * platform = 0;
  int id;
  std::vector<std::shared_ptr<Element> > children;
};

#endif
