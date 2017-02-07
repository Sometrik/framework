#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <EventHandler.h>
#include <Selection.h>

class FWPlatform;
class FWApplication;
class Command;

class Element : public EventHandler {
 public:
  Element(int _id = 0, unsigned int _flags = 0)
    : internal_id(getNextInternalId()),
    id(_id),
    flags(_flags)
    { }

  Element(const Element & other) = delete;
  Element & operator= (const Element & other) = delete;
  
  virtual void show() { }
  virtual void hide() { }
    
  virtual bool isA(const std::string & className) {
    return className == "Element";
  }  
  
  virtual int showModal() { return 0; }

  void setError(bool t) override;

  void style(const std::string & key, const std::string & value);
  void sendCommand(const Command & command);

  void onEvent(Event & ev) override;

  Element & addChild(const std::shared_ptr<Element> & element) {
    element->parent = this;
    children.push_back(element);

    if (isInitialized()) {
      element->initialize(platform);
      element->initializeChildren();
    }
    return *element;
  }  
  Element & addHeading(const std::string & text);
  Element & addText(const std::string & text);
  Element & addHorizontalLayout(int _id = 0);
  Element & addVerticalLayout(int _id = 0);

  int getId() const { return id; }
  void setId(int _id) { id = _id; }

  int getInternalId() const { return internal_id; }
  int getParentInternalId() const { return parent ? parent->getInternalId() : 0; }

  unsigned int getFlags() const { return flags; }

  void setLayoutWeight(int w) { layout_weight = w; }
  int getLayoutWeight() const { return layout_weight; }

  FWPlatform & getPlatform() { return *platform; }
  const FWPlatform & getPlatform() const { return *platform; }

  FWApplication & getApplication();
  const FWApplication & getApplication() const;
    
  std::shared_ptr<Element> getFirstChild() const {
    if (!children.empty()) {
      return children.front();
    } else {
      return std::shared_ptr<Element>(0);
    }
  }

  std::vector<std::shared_ptr<Element> > & getChildren() { return children; }

  Selection find(const std::string & q) {
    Selection s;
    find(q, s);
    return s;
  }

  void find(const std::string & q, Selection & s) {
    for (auto & c : children) {
      if (c->isA(q)) {
	s.add(c);
      }
      c->find(q, s);
    }
  }
  
  Selection getChildByInternalId(int _internal_id) {
    for (auto & c : children) {
      if (c->getInternalId() == _internal_id) {
	return Selection(c);
      } else {
	Selection s = c->getChildByInternalId(_internal_id);
	if (!s.empty()) return s;
      }
    }
    return Selection();
  }

  Selection getChildById(int _id) {
    for (auto & c : children) {
      if (c->getId() == _id) {
	return Selection(c);
      } else {
	Selection s = c->getChildById(_id);
	if (!s.empty()) return s;
      }
    }
    return Selection();
  }

  Element * getElementByInternalId(int _internal_id) { // internal use only
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
  
  void showMessageDialog(const std::string & title, const std::string & text);
  std::string showInputDialog(const std::string & title, const std::string & text);

  Element * getParent() { return parent; }
  const Element * getParent() const { return parent; }

  bool isInitialized() const { return platform != 0; }

  void removeChild(Element * c);
		   
 protected:
  virtual void initialize(FWPlatform * _platform);
  void initializeChildren();

 private:
  static int getNextInternalId() { return nextInternalId++; }

  FWPlatform * platform = 0;
  Element * parent = 0;
  int internal_id, id = 0;
  std::string name;
  std::vector<std::shared_ptr<Element> > children;
  int layout_weight = 0;
  unsigned int flags; // initialized in constructor
  bool has_error = false;

  static int nextInternalId;
};

#endif
