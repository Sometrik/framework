#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <EventHandler.h>
#include <Selection.h>
#include <Command.h>
#include <VisibilityEvent.h>
#include <Mutex.h>

#include <unordered_map>

class FWApplication;
class PlatformThread;

class Element : public EventHandler {
 public:
  Element(int _id = 0, unsigned int _flags = 0)
    : id(_id),
    flags(_flags)
    { }
  Element(const Element & other) = delete;
  Element & operator= (const Element & other) = delete; 
  ~Element();
  
  virtual void show();
  virtual void hide();  
  virtual void focus(bool enabled) { }
  virtual void refresh() { }
  virtual void text(const std::string & text) { }

  virtual bool isA(const std::string & className) const {
    return className == "*" || className == "Element";
  }  
  
  virtual bool isVisible() const {
    if (parent) return parent->isChildVisible(*this);
    else return is_visible;
  }
  
  virtual void clear() {
    sendCommand(Command(Command::CLEAR, getInternalId()));
  }

  void stop();

  void setError(bool t) override;

  Element & style(Selector s, const std::string & key, const std::string & value);
  Element & style(Selector s, const std::string & key, int value) {
    return style(s, key, std::to_string(value));
  }
  Element & style(Selector s, const std::string & key, float value) {
    return style(s, key, std::to_string(value));
  }
  Element & style(Selector s, const std::string & key, double value) {
    return style(s, key, std::to_string(value));
  }
  Element & style(const std::string & key, const std::string & value) {
    return style(Selector::NORMAL, key, value);
  }
  Element & style(const std::string & key, int value) {
    return style(Selector::NORMAL, key, value);
  }
  Element & style(const std::string & key, float value) {
    return style(Selector::NORMAL, key, value);
  }
  Element & style(const std::string & key, double value) {
    return style(Selector::NORMAL, key, value);
  }

  void begin();
  void commit();
  void sendCommand(const Command & command);

  void onEvent(Event & ev) override;
  void onVisibilityEvent(VisibilityEvent & ev) override {
    is_visible = ev.isVisible();
  }

  virtual Element & addChild(const std::shared_ptr<Element> & element) {
    element->setParent(this);
    children.push_back(element);

    if (auto ptr = thread.lock()) {
      if (ptr.get() != 0) {
	element->initialize(ptr);
	element->initializeChildren();
	element->load();
      }
    }
    return *element;
  }  
    
  int getId() const { return id; }
  void setId(int _id) { id = _id; }

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

  void reorderChildren(Element & child, unsigned int new_position) {
    Command c(Command::REORDER_CHILD, getInternalId(), child.getInternalId());
    c.setValue(new_position);
    sendCommand(c);
  }

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
  
  void showToast(const std::string & message, int duration = 0);
  void launchBrowser(const std::string & input_url);

  Element * getParent() { return parent; }
  const Element * getParent() const { return parent; }
  void setParent(Element * _parent) { parent = _parent; }

  bool isInitialized() const {
    if (auto ptr = thread.lock()) {
      return ptr.get() != 0;
    } else {
      return false;
    }
  }

  void removeChildren();
  void removeChild(Element * c);

  int createTimer(int timeout_ms);
  
  std::shared_ptr<PlatformThread> getThreadPtr() {
    return thread.lock();
  }

  virtual void initialize(std::shared_ptr<PlatformThread> _thread);

  void initializeChildren();

  static void postEventToElement(int internal_id, Event & ev) {
    auto e = getRegisteredElement(internal_id);
    if (e) {
      ev.dispatch(*e);
    }
  }

  static Element * getRegisteredElement(int internal_id) {
    MutexLocker m(mutex);
    auto it = registered_elements.find(internal_id);
    return it != registered_elements.end() ? it->second : 0;
  }
  virtual void load() { }

  int showModal(const std::shared_ptr<Element> & dialog);

 protected:
  virtual void create() { }

  virtual bool isChildVisible(const Element & child) const {
    if (!child.is_visible) return false;
    return isVisible();
  }
  
  static void registerElement(Element * e) {
    MutexLocker m(mutex);
    registered_elements[e->getInternalId()] = e;
  }

  static bool unregisterElement(Element * e) {
    MutexLocker m(mutex);
    auto it = registered_elements.find(e->getInternalId());
    if (it != registered_elements.end()) {
      registered_elements.erase(e->getInternalId());
      return true;
    } else {
      return false;
    }
  }
  
  bool is_visible = true;

 private:

  std::weak_ptr<PlatformThread> thread;
  Element * parent = 0;
  int id = 0;
  std::vector<std::shared_ptr<Element> > children;
  unsigned int flags; // initialized in constructor
  bool has_error = false;
  std::vector<Command> pendingCommands;
  bool is_enabled = true;

  static std::unordered_map<int, Element *> registered_elements;
  static Mutex mutex;
};

#endif
