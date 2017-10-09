#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <EventHandler.h>
#include <Selection.h>
#include <Command.h>
#include <VisibilityEvent.h>

#include <atomic>

class FWApplication;
class PlatformThread;
class Logger;

class Element : public EventHandler {
 public:
  Element(int _id = 0, unsigned int _flags = 0)
    : internal_id(getNextInternalId()),
    id(_id),
    flags(_flags)
    { }
  Element(const Element & other) = delete;
  Element & operator= (const Element & other) = delete; 
  ~Element();
  
  virtual void show();
  virtual void hide();
  virtual void refresh() { }
  virtual void text(const std::string & text) { }

  virtual bool isA(const std::string & className) const {
    return className == "*" || className == "Element";
  }  
  
  virtual int showModal(Element * parent) { return 0; }

  virtual bool isVisible() const {
    if (parent) return parent->isChildVisible(*this);
    else return is_visible;
  }
  
  void setError(bool t) override;
  void setEnabled(bool enabled);

  void style(const std::string & key, const std::string & value);
  void style(const std::string & key, int value) { style(key, std::to_string(value)); }

  int sendCommand(const Command & command);

  void onEvent(Event & ev) override;
  void onVisibilityEvent(VisibilityEvent & ev) override {
    is_visible = ev.isVisible();
  }

  Element & insertChild(const std::shared_ptr<Element> & element) {
    element->parent = this;
    children.insert(children.begin(), element);

    if (isInitialized()) {
      element->initialize(thread);
      element->initializeChildren();
    }
    return *element;
  }  

  Element & addChild(const std::shared_ptr<Element> & element) {
    element->setParent(this);
    children.push_back(element);

    if (isInitialized()) {
      element->initialize(thread);
      element->initializeChildren();
    }
    return *element;
  }  
    
  Element & addText(const std::string & text);
  Element & addHorizontalLayout(int _id = 0);
  Element & addVerticalLayout(int _id = 0);

  int getId() const { return id; }
  void setId(int _id) { id = _id; }

  int getInternalId() const { return internal_id; }
  int getParentInternalId() const { return parent ? parent->getInternalId() : 0; }

  unsigned int getFlags() const { return flags; }

  FWApplication & getApplication();
  const FWApplication & getApplication() const;
    
  std::shared_ptr<Element> getFirstChild() const {
    if (!children.empty()) {
      return children.front();
    } else {
      return std::shared_ptr<Element>(0);
    }
  }

  std::shared_ptr<Element> getLastChild() const {
    if (!children.empty()) {
      return children.back();
    } else {
      return std::shared_ptr<Element>(0);
    }
  }

  std::vector<std::shared_ptr<Element> > & getChildren() { return children; }
  const std::vector<std::shared_ptr<Element> > & getChildren() const { return children; }

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
  
  void showMessageDialog(const std::string & title, const std::string & text);
  std::string showInputDialog(const std::string & title, const std::string & text);
  void showToast(const std::string & message, int duration = 0);
  void launchBrowser(const std::string & input_url);

  Element * getParent() { return parent; }
  const Element * getParent() const { return parent; }
  void setParent(Element * _parent) { parent = _parent; }
  size_t size() const { return children.size(); }
  bool isInitialized() const { return thread != 0; }

  void removeChild(Element * c);

  int createTimer(int timeout_ms);

  PlatformThread & getThread() { return *thread; }
  const PlatformThread & getThread() const { return *thread; }

  virtual void initialize(PlatformThread * _thread);

  void initializeChildren();

  Logger & getLogger();

 protected:
  virtual void create() = 0;
  virtual void load() { }

  virtual bool isChildVisible(const Element & child) const {
    if (!child.is_visible) return false;
    else if (parent) return parent->isChildVisible(*this);
    else return true;
  }

  bool is_visible = true;

 private:
  static int getNextInternalId() { return nextInternalId.fetch_add(1); }

  PlatformThread * thread = 0;
  Element * parent = 0;
  int internal_id, id = 0;
  std::vector<std::shared_ptr<Element> > children;
  unsigned int flags; // initialized in constructor
  bool has_error = false;
  std::vector<Command> pendingCommands;
  bool is_enabled = true;
  std::shared_ptr<Logger> logger;

  static std::atomic<int> nextInternalId;
};

#endif
