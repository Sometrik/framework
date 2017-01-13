#ifndef _BINDABLE_H_
#define _BINDABLE_H_

#include <memory>
#include <vector>

#include <shims.h>

class Bind {
 public:
  Bind() { }
  virtual ~Bind() { }
  
  Bind(const Bind & other) = delete;
  Bind & operator =(const Bind & other) = delete;

  virtual void call() = 0;
  virtual void call(bool t) = 0;
  virtual void call(int i) = 0;
  virtual void call(const std::string & s) = 0;  
};

template<class T>
class PointerBind : public Bind {
 public:
  PointerBind(T * _ptr) : ptr(_ptr) { }

  void call() override { call(true); }
  void call(bool t) override { assignBoundVar(*ptr, t); }
  void call(int i) override { assignBoundVar(*ptr, i); }
  void call(const std::string & s) override { assignBoundVar(*ptr, s); }

 private:
  T * ptr;
};

class Bindable : public Bind {
 public:
  Bindable() { }
  
  template<class T>
  Bindable & bind(T * arg) {
    bound_objects.push_back(std::make_shared<PointerBind<T> >(arg));
    return *this;
  }

  void call() override { call(true); }
  void call(bool t) override { call(t ? 1 : 0); }
  void call(int i) override { }
  void call(const std::string & s) override { }

 protected:
  std::vector<std::shared_ptr<Bind> > & getBoundObjects() { return bound_objects; }

  template<class T>
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
  std::vector<std::shared_ptr<Bind> > bound_objects;
};

#endif
