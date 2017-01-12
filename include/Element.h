#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <EventHandler.h>

#include <string>
#include <cstring>
#include <vector>

inline void assignBoundVar(std::string & a, const std::string & b) { a = b; }
inline void assignBoundVar(int & a, const std::string & b) { a = atoi(b.c_str()); }
inline void assignBoundVar(bool & a, const std::string & b) { a = atoi(b.c_str()) != 0; }

inline void assignBoundVar(std::string & a, bool b) { a = std::to_string(b); }
inline void assignBoundVar(int & a, bool b) { a = int(b); }
inline void assignBoundVar(bool & a, bool b) { a = b; }

class Bind {
 public:
  Bind() { }
  virtual ~Bind() { }
  
  Bind(const Bind & other) = delete;
  Bind & operator =(const Bind & other) = delete;

  virtual void call(bool t) = 0;
  virtual void call(const std::string & s) = 0;  
};

template<class T>
class PointerBind : public Bind {
 public:
  PointerBind(T * _ptr) : ptr(_ptr) { }

  void call(bool t) override { assignBoundVar(*ptr, t); }
  void call(const std::string & s) override { assignBoundVar(*ptr, s); }

 private:
  T * ptr;
};

#include <memory>
#include <string>

class FWPlatform;
class FWApplication;
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

  template<class T>
  Element & bind(T * arg) {
    bound_objects.push_back(std::make_shared<PointerBind<T> >(arg));
    return *this;
  }

protected:  
  bool isInitialized() const { return internal_id != 0; }
  std::vector<std::shared_ptr<Bind> > & getBoundObjects() { return bound_objects; }

 private:
  FWPlatform * platform = 0;
  Element * parent = 0;
  int internal_id = 0, id = 0;
  std::string name;
  std::vector<std::shared_ptr<Element> > children;
  int layout_weight = 0;
  unsigned int flags; // initialized in constructor
  std::vector<std::shared_ptr<Bind> > bound_objects;
};

#endif
