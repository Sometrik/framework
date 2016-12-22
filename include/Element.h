#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <EventHandler.h>

#include <memory>
#include <vector>
#include <string>

class FWPlatform;
class Command;
class LinearLayout;

class Element : public EventHandler {
 public:
  Element(int _id = 0, unsigned int _flags = 0) : id(_id), flags(_flags) { }

  Element(const Element & other) = delete;
  Element & operator= (const Element & other) = delete;

  virtual void initialize(FWPlatform * _platform);
  virtual void initializeContent() { }
  
  virtual void show() { }
  virtual int showModal() { return 0; }
  virtual void sendCommand(const Command & command);

  void onEvent(Event & ev) override;

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
  void setId(int _id) { id = _id; }

  int getInternalId() const { return internal_id; }
  int getParentInternalId() const { return parent ? parent->getInternalId() : 0; }

  unsigned int getFlags() const { return flags; }

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
	Element * e = c->getElementByInternalId(_internal_id);
	if (e) return e;
      }
      return 0;
    }
  }

  const Element * getElementById(int _id) const {
    if (_id == id) {
      return this;
    } else {
      for (auto & c : children) {
	const Element * e = c->getElementById(_id);
	if (e) return e;
      }
      return 0;
    }
  }

  Element * getElementById(int _id) {
    if (_id == id) {
      return this;
    } else {
      for (auto & c : children) {
	Element * e = c->getElementById(_id);
	if (e) return e;
      }
      return 0;
    }
  }
  
  void showMessageDialog(const std::string & title, const std::string & text);
  std::string showInputDialog(const std::string & title, const std::string & text);

  Element * getParent() { return parent; }
  const Element * getParent() const { return parent; }
    
protected:  
  bool isInitialized() const { return internal_id != 0; }

 private:
  FWPlatform * platform = 0;
  Element * parent = 0;
  int internal_id = 0, id = 0;
  std::string name;
  std::vector<std::shared_ptr<Element> > children;
  unsigned int flags;
};

#endif
