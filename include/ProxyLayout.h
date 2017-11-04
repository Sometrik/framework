#ifndef _PROXYLAYOUT_H_
#define _PROXYLAYOUT_H_

#include <Element.h>

class ProxyLayout : public Element {
public:
  ProxyLayout(int _id = 0) : Element(_id) { }

  bool isA(const std::string & className) const override {
    if (className == "ProxyLayout") return true;
    return Element::isA(className);
  }
  
  void clear() {
    current_keys.clear();
  }

  void addProxy(const std::string & key, const std::string & data) {
    int pos = (int)current_keys.size();

    current_keys.insert(key);

    if (pos < max_visible_count) {
      auto orig = getContent(key);
      if (orig.get()) {
	updateContent(*orig, key, data);
	
	Command c(Command::REORDER_CHILD, getInternalId(), orig->getInternalId());
	c.setValue(pos);
	sendCommand(c);
      } else {
	auto e = createContent(key, data);
	content[key] = e;
	addChild(e);
      }
    }
  }

  void flush() {
    for (auto it = content.begin(); it != content.end(); ) {
      if (current_keys.count(it->first)) {
        it++;
      } else {
        removeChild(it->second.get());
        it = content.erase(it);
      }
    }
  }

protected:
  virtual std::shared_ptr<Element> createContent(const std::string & key, const std::string & data) = 0;
  virtual void updateContent(Element & e, const std::string & key, const std::string & data) = 0;
  
  std::shared_ptr<Element> getContent(const std::string & key) {
    auto it = content.find(key);
    if (it != content.end()) {
      return it->second;
    } else {
      return std::shared_ptr<Element>(0);
    }
  }

  void create() override {
    Command c(Command::CREATE_LINEAR_LAYOUT, getParentInternalId(), getInternalId());
    c.setValue(1);
    sendCommand(c);
  }
  
  size_t getProxyCount() const { return current_keys.size(); }

private:
  int max_visible_count = 25;
  std::unordered_map<std::string, std::shared_ptr<Element> > content;
  std::unordered_set<std::string> current_keys;
};

#endif
