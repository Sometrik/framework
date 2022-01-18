#ifndef _FWPREFERENCES_H_
#define _FWPREFERENCES_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stdexcept>

class FWPreferences {
 public:
  FWPreferences() { }

  const std::string & getText(const std::string & key) const {
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    else return empty_string;
  }

  const std::string & getText(const std::string & key, const std::string & defaultValue) const {
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    else return defaultValue;
  }

  void deleteKey(const std::string & key) {
    data.erase(key);
    deletedKeys.insert(key);
  }

  int getInt(const std::string & key, int defaultValue = 0) const {
    try {
      return std::stoi(getText(key, std::to_string(defaultValue)));
    } catch (std::invalid_argument & e) {
      return 0;
    }
  }

  long long getLong(const std::string & key, long long defaultValue = 0) const {
    try {
      return std::stoll(getText(key, std::to_string(defaultValue)));
    } catch (std::invalid_argument & e) {
      return 0;
    }
  }

  void setText(const std::string & key, const std::string & value) {
    deletedKeys.erase(key);
    data[key] = value;
    changedKeys.insert(key);
  }
  
  void setInt(const std::string & key, int value) {
    setText(key, std::to_string(value));
  }

  void setLong(const std::string & key, long long value) {
    setText(key, std::to_string(value));
  }

  bool empty() const { return data.empty(); }
  size_t size() const { return data.size(); }

  std::unordered_map<std::string, std::string> getChanges() const {
    std::unordered_map<std::string, std::string> r;
    for (auto & key : changedKeys) {
      r[key] = getText(key);
    }
    return r;
  }

  const std::unordered_set<std::string> & getDeletedKeys() {
    return deletedKeys;
  }
  
  void clearChanges() { changedKeys.clear(); deletedKeys.clear(); }
  
  std::vector<std::string> getKeys() const {
    std::vector<std::string> r;
    for (auto & d : data) {
      r.push_back(d.first);
    }
    return r;
  }

 private:
  std::unordered_map<std::string, std::string> data;
  std::string empty_string;
  std::unordered_set<std::string> changedKeys;
  std::unordered_set<std::string> deletedKeys;
};

#endif
