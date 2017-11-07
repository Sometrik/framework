#ifndef _PROXYLAYOUT_H_
#define _PROXYLAYOUT_H_

#include <Element.h>
#include <ScrollChangedEvent.h>

template <class T1, class T2>
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

  void addProxy(const T1 & key, const T2 & data) {
    unsigned int pos = all_keys.size();
    all_keys.push_back(std::make_pair(key, data));

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

      if (all_keys.size() > max_visible_count) {
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
  }

protected:
  void showKey(unsigned int pos, const T1 & key, const T2 & data) {
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
  
  virtual std::shared_ptr<Element> createContent(const T1 & key, const T2 & data) = 0;
  virtual void updateContent(Element & e, const T1 & key, const T2 & data) = 0;
  
  std::shared_ptr<Element> getContent(const T1 & key) {
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
  unsigned int max_visible_count;
  unsigned int current_content_height = 0;
  std::unordered_map<T1, std::shared_ptr<Element> > content;
  std::unordered_set<T1> visible_keys;
  std::vector<std::pair<T1, T2> > all_keys;
};

#endif
