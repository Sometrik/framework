#ifndef _PROXYLAYOUT_H_
#define _PROXYLAYOUT_H_

#include <Element.h>
#include <ScrollChangedEvent.h>

template <class T1, class T2, class T3>
class ProxyLayout : public T3 {
public:
  ProxyLayout(int _initial_visible_count) : max_visible_count(_initial_visible_count) { }

  bool isA(const std::string & className) const override {
    if (className == "ProxyLayout") return true;
    return T3::isA(className);
  }
  
  void reset() {
    visible_keys.clear();
    all_keys.clear();
  }

  void addProxy(const T1 & key, const T2 & data) {
    size_t pos = all_keys.size();
    all_keys.push_back(std::make_pair(key, data));

    if (pos < max_visible_count) {
      showKey(pos, key, data);
    }
  }

  void flush() {
    for (auto it = content.begin(); it != content.end(); ) {
      if (isVisible(it->first)) {
        it++;
      } else {
        Element::removeChild(it->second.get());
        it = content.erase(it);
      }
    }
    Element::sendCommand(Command(Command::FLUSH_VIEW, Element::getInternalId()));
  }

  void onScrollChangedEvent(ScrollChangedEvent & ev) override {
    if (ev.getHeight() != current_content_height && ev.getScrollRem() < 10) {
      current_content_height = ev.getHeight();

      if (all_keys.size() > max_visible_count) {
        Element::begin();
        unsigned int c = 10;
        for (unsigned int i = max_visible_count; i < max_visible_count + c && i < all_keys.size(); i++) {
          auto & p = all_keys[i];
          showKey(i, p.first, p.second);
        }
        max_visible_count += c;
        Element::commit();
      }
    }
  }

  size_t getProxyCount() const { return all_keys.size(); }

protected:
  T1 getKeyForInternalId(int id) const {
    for (auto & row : content) {
      if (row.second->getInternalId() == id) {
	return row.first;
      }
    }
    return T1();
  }

  bool isVisible(const T1 & key) const {
    return visible_keys.count(key);
  }

  void showKey(size_t pos, const T1 & key, const T2 & data) {
    visible_keys.insert(key);

    auto orig = getContent(key);
    if (orig.get()) {
      updateContent(*orig, key, data);
      Element::reorderChildren(*orig, (unsigned int)pos);
    } else {
      auto e = createContent(key, data);
      content[key] = e;
      Element::addChild(e);
      Element::reorderChildren(*e, (unsigned int)pos);
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
  
private:
  unsigned int max_visible_count;
  unsigned int current_content_height = 0;
  std::unordered_map<T1, std::shared_ptr<Element> > content;
  std::unordered_set<T1> visible_keys;
  std::vector<std::pair<T1, T2> > all_keys;
};

#endif
