#ifndef _FWPREFERENCES_H_
#define _FWPREFERENCES_H_

#include <map>
#include <string>

class FWPreferences {
 public:
  FWPreferences() { }

  void setValue(const std::string & key, const std::string & value) {
    data[key] = value;
  }

  const std::string & getText(const char * key) const {
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    else return empty_string;
  }

  const std::string & getText(const std::string & key, const std::string & defaultValue) const {
    auto it = data.find(key);
    if (it != data.end()) return it->second;
    else return defaultValue;
  }

  int getInt(const char * key, int defaultValue = 0) {
    return stoi(getText(key, to_string(defaultValue)));
  }

  bool empty() const { return data.empty(); }
  size_t size() const { return data.size(); }
  
 private:
  std::map<std::string, std::string> data;
  std::string empty_string;
};

#endif
