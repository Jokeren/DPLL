#ifndef TINY_SAT_ASSIGNMENT_H
#define TINY_SAT_ASSIGNMENT_H

#include <sstream>

#include "common.h"
#include "proposition.h"
#include "path.h"


namespace tiny_sat {

class Assignment {
 public:
  Assignment(size_t props = 1) {
    truth_.resize(props + 1);
    truth_.fill(EVAL_UNDECIDED);
  }

  Evaluation get(const Proposition &prop) const {
    return truth_[prop];
  }

  void assign(const Proposition &prop, Evaluation truth) {
    truth_[prop] = truth;
  }

  void remove(const Proposition &prop) {
    truth_[prop] = EVAL_UNDECIDED;
  }

  std::string to_string() const {
    std::stringstream ss;
    for (size_t i = 1; i <= truth_.size(); ++i) {
      if (truth_[i] == EVAL_SAT) {
        ss << i << " ";
      }
    }
    return ss.str();
  }

 private:
  Path<Evaluation> truth_;
};

}  // namespace tiny_sat

#endif

