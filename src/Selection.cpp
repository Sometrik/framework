#include <Selection.h>
#include <Element.h>

using namespace std;

Selection &
Selection::show() {
  for (auto & e : data) {
    e->show();
  }
  return *this;
}

Selection &
Selection::hide() {
  for (auto & e : data) {
    e->hide();
  }
  return *this;
}

Selection &
Selection::remove() {
  for (auto & e : data) {
    Element * p = e->getParent();
    if (p) {
      p->removeChild(e.get());
    }
  }
  data.clear();
  return *this;
}

Selection &
Selection::refresh() {
  for (auto & e : data) {
    e->refresh();
  }
  return *this;
}

Selection &
Selection::style(Selector selector, const std::string & key, const std::string & value) {
  for (auto & e : data) {
    e->style(selector, key, value);
  }
  return *this;
}

Selection &
Selection::text(const std::string & text) {
  for (auto & e : data) {
    e->text(text);
  }
  return *this;
}

Selection
Selection::parent() const {
  Selection r;
  for (auto & e : data) {
    // Get the shared_ptr from grandparents
    Element * parent = e->getParent();
    if (parent) {
      Element * grandparent = parent->getParent();
      if (grandparent) {
        for (auto & sibling : grandparent->getChildren()) {
          if (sibling.get() == parent) {
            r.add(sibling);
          }
        }
      }
    }
  }
  return r;
}

Selection
Selection::parents() const {
  Selection r;
  auto queue = data;
  for (size_t i = 0; i < queue.size(); i++) {
    auto & e = queue[i];
    // Get the shared_ptr from grandparents
    Element * parent = e->getParent();
    if (parent) {
      Element * grandparent = parent->getParent();
      if (grandparent) {
        for (auto & sibling : grandparent->getChildren()) {
          if (sibling.get() == parent) {
            r.add(sibling);
            queue.push_back(sibling);
          }
        }
      }
    }
  }
  return r;
}

Selection
Selection::children() const {
  vector<shared_ptr<Element> > all_children;
  for (auto & e : data) {
    all_children.insert(all_children.end(), e->getChildren().begin(), e->getChildren().end());
  }
  return Selection(all_children);
}
