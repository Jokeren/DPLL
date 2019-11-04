#ifndef TINY_SAT_ASSIGNMENT_H
#define TINY_SAT_ASSIGNMENT_H

#include <map>
#include <sstream>

#include "proposition.h"


namespace tiny_sat {

class Assignment {
 public:
  Assignment() {}

  bool get(const Proposition &prop) const {
    return truth_.find(prop) != truth_.end();
  }

  void assign(const Proposition &prop) {
    truth_[prop] = true;
  }

  void remove(const Proposition &prop) {
    truth_[prop] = false;
  }

  std::string to_string() {
    std::stringstream ss;
    for (auto iter : truth_) {
      ss << iter.first.id() << ": " << iter.second << std::endl;
    }
    return ss.str();
  }

 private:
  std::map<Proposition, bool> truth_;
};

}  // namespace tiny_sat

#endif

