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

  bool call() override { return call(true); }
  bool call(bool t) override { assignBoundVar(*ptr, t); return true; }
  bool call(int i) override { assignBoundVar(*ptr, i); return true; }
  bool call(const std::string & s) override { assignBoundVar(*ptr, s); return true; }

 private:
  T * ptr;
};

class BoundVoidFunc : public Bindable {
 public:
  BoundVoidFunc(std::function<bool ()> const & _func) : func(_func) { }

  bool call() override { return func(); }
  bool call(bool t) override { return func(); }
  bool call(int i) override { return func(); }
  bool call(const std::string & s) override { return func(); }

 private:
  std::function<bool (void)> const & func;
};

template<class T>
class BoundFunc : public Bindable {
 public:
  BoundFunc(const std::function<bool (T)> & _func) : func(_func) { }

  bool call() override { return call(true); }
  bool call(bool t) override {
    T value;
    assignBoundVar(value, t);
    return func(value);
  }
  bool call(int i) override {
    T value;
    assignBoundVar(value, i);
    return func(value);
  }
  bool call(const std::string & s) override {
    T value;
    assignBoundVar(value, s);
    return func(value);
  }

 private:
  std::function<bool (T)> func;
};

class Notifier : public Bindable {
 public:
  template<class T>
  Notifier & bind(T * arg) {
    return bind(std::make_shared<BoundPtr<T> >(arg));
  }

  Notifier & bindFunc(const std::function<bool ()> & arg) {
    return bind(std::make_shared<BoundVoidFunc>(arg));
  }

  Notifier & bindFunc(const std::function<bool (std::string)> & arg) {
    return bind(std::make_shared<BoundFunc<std::string> >(arg));
  }

  Notifier & bindFunc(const std::function<bool (bool)> & arg) {
    return bind(std::make_shared<BoundFunc<bool> >(arg));
  }

  Notifier & bind(const std::shared_ptr<Bindable> & bindable) {
    bound_objects.push_back(bindable);
    return *this;
  }

 protected:
  std::vector<std::shared_ptr<Bindable> > & getBoundObjects() {
    return bound_objects;
  }

  virtual void setError(bool t) { }

  void notify() {
    bool e = false;
    for (auto & o : getBoundObjects()) {
      if (!o->call()) {
	e = true;
	break;
      }
    }
    setError(e);
  }

  template<class T>
  void notify(T arg) {
    bool e = false;
    for (auto & o : getBoundObjects()) {
      if (!o->call(arg)) {
	e = true;
	break;
      }
    }
    setError(e);
  }

 private:
  std::vector<std::shared_ptr<Bindable> > bound_objects;
};

#endif
