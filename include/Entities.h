#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include <string>
#include <unordered_map>

class Entities {
 public:
  static Entities & getInstance() {
    if (!instance) {
      instance = new Entities();
    }
    return *instance;
  }

  int convertEntity(const std::string & entity);

 private:
  Entities();
  Entities(const Entities & other) = delete;
  ~Entities() { }
  Entities & operator=(const Entities & other) = delete;

  std::unordered_map<std::string, int> entities;
  
  static Entities * instance;
};

#endif
