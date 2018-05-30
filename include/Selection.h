#ifndef _SELECTION_H_
#define _SELECTION_H_

#include <Selector.h>

#include <memory>
#include <vector>
#include <string>

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
  Selection & clear();
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

  Selection & add(const std::shared_ptr<Element> & e) { data.push_back(e); return *this; }
    
  bool empty() const { return data.empty(); }
  size_t size() const { return data.size(); }

  Element & front() { return *(data.front()); }
  
 private:
  std::vector<std::shared_ptr<Element> > data;
};

#endif
