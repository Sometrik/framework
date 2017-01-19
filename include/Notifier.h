#ifndef _NOTIFIER_H_
#define _NOTIFIER_H_

#include <Bindable.h>

#include <memory>
#include <vector>

#include <shims.h>

template<class T>
class BoundPtr : public Bindable {
 public:
  BoundPtr(T * _ptr) : ptr(_ptr) { }

  void call() override { call(true); }
  void call(bool t) override { assignBoundVar(*ptr, t); }
  void call(int i) override { assignBoundVar(*ptr, i); }
  void call(const std::string & s) override { assignBoundVar(*ptr, s); }

 private:
  T * ptr;
};

class Notifier : public Bindable {
 public:
  template<class T>
  Notifier & bind(T * arg) {
    return bind(std::make_shared<BoundPtr<T> >(arg));
  }
  
  Notifier & bind(const std::shared_ptr<Bindable> & bindable) {
    bound_objects.push_back(bindable);
    return *this;
  }

 protected:
  std::vector<std::shared_ptr<Bindable> > & getBoundObjects() {
    return bound_objects;
  }

  void notify() {
    for (auto & o : getBoundObjects()) {
      o->call();
    }
  }

  template<class T>
  void notify(T arg) {
    for (auto & o : getBoundObjects()) {
      o->call(arg);
    }
  }

 private:
  std::vector<std::shared_ptr<Bindable> > bound_objects;
};

#endif
