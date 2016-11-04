#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <memory>
#include <vector>

class TouchEvent;
class CommandEvent;
class DrawEvent;
class PurchaseEvent;
class SysEvent;
class LinearLayout;

class FWPlatform;
class Message;

class Element {
 public:
  Element() { }
  virtual ~Element() { }

  virtual void initialize(FWPlatform * _platform);
  virtual void initializeContent() { }

  virtual void onDrawEvent(DrawEvent & ev) { }
  virtual void onCommandEvent(CommandEvent & ev) { }
  virtual bool onTouchEvent(TouchEvent & ev) { return false; }
  virtual void onPurchaseEvent(PurchaseEvent & ev) { }
  virtual void onSysEvent(SysEvent & ev) { }
  
  virtual void onCmdLine(int argc, char *argv[]) { }
  virtual bool onKeyPress(char c, double timestamp, int x, int y) { return false; }
  virtual bool onUpdate(double timestamp) { return false; }
  virtual bool onResize(int _logical_width, int _logical_height, int _actual_width, int _actual_height) { return false; }
  virtual bool flushTouches(int mode, double timestamp) { return false; }
  virtual bool onShake(double timestamp) { return false; }

  virtual void show() { }
    
  Element & addChild(const std::shared_ptr<Element> & element) {
    element->parent = this;
    element->initialize(platform);
    children.push_back(element);
    element->initializeContent();
    return *element;
  }
  Element & addChild(const std::string & text);
  LinearLayout & addHorizontalLayout();
  LinearLayout & addVerticalLayout();

  int getId() const { return id; }
  int setId(int _id) const { id = _id; }

  int getInternalId() const { return id; }
  int getParentInternalId() const { return parent ? parent->getInternalId() : 0; }
  
  void sendMessage(const Message & message);
  
  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }
  
  std::shared_ptr<Element> getFirstChild() const {
    if (!children.empty()) {
      return children.front();
    } else {
      return std::shared_ptr<Element>(0);
    }
  }

  std::vector<std::shared_ptr<Element> > & getChildren() { return children; }

  const Element * getElementByInternalId(int _internal_id) const {
    if (_internal_id == internal_id) {
      return this;
    } else {
      for (auto & c : children) {
	const Element * e = c->getElementByInternalId(_internal_id);
	if (e) return e;
      }
      return 0;
    }
  }

  Element * getElementByInternalId(int _internal_id) {
    if (_internal_id == internal_id) {
      return this;
    } else {
      for (auto & c : children) {
	const Element * e = c->getElementByInternalId(_internal_id);
	if (e) return e;
      }
      return 0;
    }
  }

protected:
  bool isInitialized() const { return parent != 0; }

 private:
  FWPlatform * platform = 0;
  Element * parent = 0;
  int internal_id = 0, id = 0;
  std::string name;
  std::vector<std::shared_ptr<Element> > children;
};

#endif
