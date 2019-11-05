#ifndef TINY_SAT_ASSIGNMENT_H
#define TINY_SAT_ASSIGNMENT_H

#include <set>
#include <sstream>

#include "common.h"
#include "proposition.h"


namespace tiny_sat {

class Assignment {
 public:
  Assignment() {}

  Evaluation get(const Proposition &prop) const {
    auto iter = truth_.find(prop);
    if (iter == truth_.end()) {
      // prop not assigned
      return EVAL_UNDECIDED;
    } else {
      // prop assigned
      return iter->second ? EVAL_SAT : EVAL_UNSAT;
    }
  }

  void assign(const Proposition &prop, bool truth) {
    truth_[prop] = truth;
  }

  void remove(const Proposition &prop) {
    truth_.erase(prop);
  }

  std::string to_string() const {
    std::stringstream ss;
    for (auto iter : truth_) {
      if (iter.second) {
        ss << iter.first.id() << " : " << iter.second << std::endl;
      }
    }
    return ss.str();
  }

 private:
  std::map<Proposition, bool> truth_;
};

}  // namespace tiny_sat

#endif

