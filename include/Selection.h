#ifndef _SELECTION_H_
#define _SELECTION_H_

#include <Selector.h>

#include <memory>
#include <vector>

class Element;

class Selection {
 public:
  Selection() { }
  Selection(const std::vector<std::shared_ptr<Element> > & _data)
    : data(_data) { }
  Selection(const std::shared_ptr<Element> & e) {
    add(e);
  }

  Selection & show();
  Selection & hide();
  Selection & remove();
  Selection & refresh();
  Selection & setEnabled(bool enabled);
  Selection & text(const std::string & text);
  Selection & style(Selector selector, const std::string & key, const std::string & value);
  Selection & style(Selector selector, const std::string & key, int value) {
    style(key, std::to_string(value));
    return *this;
  }
  Selection & style(const std::string & key, const std::string & value) {
    style(Selector::NORMAL, key, value);
    return *this;
  }
  Selection & style(const std::string & key, int value) {
    style(Selector::NORMAL, key, value);
    return *this;
  }

  Selection parent() const;
  Selection parents() const;
  Selection children() const;

  void add(const std::shared_ptr<Element> & e) { data.push_back(e); }
  void clear() { data.clear(); }
    
  bool empty() const { return data.empty(); }
  size_t size() const { return data.size(); }

  Element & front() { return *(data.front()); }
  
 private:
  std::vector<std::shared_ptr<Element> > data;
};

#endif
