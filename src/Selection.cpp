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
Selection::style(const std::string & key, const std::string & value) {
  for (auto & e : data) {
    e->style(key, value);
  }
  return *this;
}

Selection
Selection::parent() const {
  // TODO: return a selection of parents of current selection
  return Selection();
}

Selection
Selection::parents() const {
  // TODO: return a selection of all parents of current selection
  return Selection();
}

Selection
Selection::children() const {
  vector<shared_ptr<Element> > all_children;
  for (auto & e : data) {
    all_children.insert(all_children.end(), e->getChildren().begin(), e->getChildren().end());
  }
  return Selection(all_children);
}
