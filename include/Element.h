#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <memory>
#include <vector>

class TouchEvent;
class CommandEvent;
class FWPlatformBase;
class Message;

class Element {
 public:
  Element() {
  }

  virtual ~Element() { }


  virtual void initialize(FWPlatformBase * _platform);
  virtual void initializeContent() { }
  virtual void onDraw() { }
  virtual void onShutdown() { }
  virtual void onMemoryWarning() { }
  virtual void onCommandEvent(CommandEvent & ev) { }

      virtual void onCmdLine(int argc, char *argv[]) { }

      virtual bool onKeyPress(char c, double timestamp, int x, int y) { return false; }
      virtual bool onUpdate(double timestamp) { return false; }
      virtual bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) { return false; }

      virtual bool onTouchEvent(TouchEvent & touchEvent) { return false; }
      virtual bool flushTouches(int mode, double timestamp) { return false; }
      virtual bool onShake(double timestamp) { return false; }

      Element & addChild(const std::shared_ptr<Element> & element) {
	element->parent = this;
        element->initialize(platform);
        children.push_back(element);
	element->initializeContent();
	return *element;
      }

      int getId() const { return id; }
      int getParentId() const { return parent ? parent->getId() : 0; }

      void sendMessage(const Message & message);

      FWPlatformBase & getPlatform() { return *platform; }
      const FWPlatformBase & getPlatform() const { return *platform; }

      std::shared_ptr<Element> getFirstChild() const {
	if (!children.empty()) {
	  return children.front();
	} else {
	  return std::shared_ptr<Element>(0);
	}
      }

protected:
  FWPlatformBase * platform = 0;
  Element * parent = 0;
  int id = 0;
  std::string name;
  std::vector<std::shared_ptr<Element> > children;
};

#endif
