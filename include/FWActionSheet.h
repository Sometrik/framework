#ifndef _FWACTIONSHEET_H_
#define _FWACTIONSHEET_H_

#include <string>
#include <vector>

class FWOption {
 public:
 FWOption(int _id, const std::string & _text) : id(_id), text(_text) { }

  int getId() const { return id; }
  const std::string & getText() const { return text; }
  
 private:
  int id;
  std::string text;
};

class FWActionSheet {
 public:
  FWActionSheet(const std::string & _title)
    : title(_title) { }

  void addOption(int id, const std::string & name) {
    options.push_back(FWOption(id, name));
  }

  const std::string & getTitle() const { return title; }
  const std::vector<FWOption> & getOptions() const { return options; }
  
 private:
  std::string title;
  std::vector<FWOption> options;
};
#endif
