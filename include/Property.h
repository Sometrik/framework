#ifndef _PROPERTY_H_
#define _PROPERTY_H_

#include <Notifier.h>

class PropertyBase : public Notifier {
 public:
  PropertyBase() { }

  virtual std::string getStringValue() const = 0;
  virtual bool getBoolValue() const = 0;

  void set(bool t) { call(t); }
  void set(int i) { call(i); }
  void set(const std::string & s) { call(s); }
};

template<class T>
class Property : public PropertyBase {
 public:
  Property() : data() { }
  Property(const T & _data) : data(_data) { }
  Property(const Property<T> & _other) : data(_other.data) { }

  Property & operator=(const Property<T> & _other) {
    data = _other.data;
    notify(data);
    return *this;
  }

  Property & operator=(const T & _other) {
    data = _other;
    notify(data);
    return *this;
  }

  T & get() { return data; }
  const T & get() const { return data; }

  void call() override { }
  void call(bool t) override { assignBoundVar(data, t); }
  void call(int i) override { assignBoundVar(data, i); }
  void call(const std::string & s) { assignBoundVar(data, s); }
  
  std::string getStringValue() const override {
    std::string s;
    assignBoundVar(s, data);
    return s;
  }

  bool getBoolValue() const override {
    bool b;
    assignBoundVar(b, data);
    return b;
  }

 private:
  T data;
};

class NullProperty : public PropertyBase {
 public:
  NullProperty() { }

  void call() override { }
  void call(bool t) override { }
  void call(int i) override { }
  void call(const std::string & s) { }

  std::string getStringValue() const override { return ""; }
  bool getBoolValue() const override { return false; }
};

#endif
