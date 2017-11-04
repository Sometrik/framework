#ifndef _PROXYLAYOUT_H_
#define _PROXYLAYOUT_H_

#include <Element.h>
#include <ScrollChangedEvent.h>

class ProxyLayout : public Element {
public:
  ProxyLayout(int _initial_visible_count, int _id = 0) : Element(_id), max_visible_count(_initial_visible_count) { }

  bool isA(const std::string & className) const override {
    if (className == "ProxyLayout") return true;
    return Element::isA(className);
  }
  
  void clear() {
    visible_keys.clear();
    all_keys.clear();
  }

  void addProxy(const std::string & key, const std::string & data) {
    unsigned int pos = all_keys.size();
    all_keys.push_back(std::pair<std::string, std::string>(key, data));

    if (pos < max_visible_count) {
      showKey(pos, key, data);
    }
  }

  void flush() {
    for (auto it = content.begin(); it != content.end(); ) {
      if (visible_keys.count(it->first)) {
        it++;
      } else {
        removeChild(it->second.get());
        it = content.erase(it);
      }
    }
  }

  void onScrollChangedEvent(ScrollChangedEvent & ev) override {
    if (ev.getHeight() != current_content_height && ev.getScrollRem() < 10) {
      current_content_height = ev.getHeight();

      begin();
      unsigned int c = 10;
      for (unsigned int i = max_visible_count; i < max_visible_count + c && i < all_keys.size(); i++) {
	auto & p = all_keys[i];
	showKey(i, p.first, p.second);
      }
      max_visible_count += c;
      commit();
    }
  }

protected:
  void showKey(unsigned int pos, const std::string & key, const std::string & data) {
    visible_keys.insert(key);
      
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
  
  size_t getProxyCount() const { return all_keys.size(); }

private:
  int max_visible_count;
  int current_content_height = 0;
  std::unordered_map<std::string, std::shared_ptr<Element> > content;
  std::unordered_set<std::string> visible_keys;
  std::vector<std::pair<std::string, std::string> > all_keys;
};

#endif
