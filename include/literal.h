#ifndef TINY_SAT_LITERAL_H
#define TINY_SAT_LITERAL_H

#include "proposition.h"
#include "assignment.h"

#include <cassert>


namespace tiny_sat {

class Literal {
 public:
  Literal() : propsition_(0), positive_(true) {}

  Literal(const Proposition &prop, bool positive = true) :
    propsition_(prop), positive_(positive) {}

  bool positive() const {
    // Leave zero as an undefined proposition marker
    assert(propsition_ != 0);
    return positive_;
  }

  Evaluation eval(const Assignment &assignment) const {
    Evaluation truth = assignment.get(propsition_);
    if (truth == EVAL_UNDECIDED) {
      return truth;
    }
    return positive_ ? truth : (Evaluation)(1 - truth);
  }

  unsigned int id() const {
    // Leave zero as an undefined proposition marker
    assert(propsition_ != 0);
    return propsition_;
  }

  const Proposition prop() const {
    return propsition_;
  }

  std::string to_string() const {
    if (positive_) {
      return std::to_string(propsition_);
    } else {
      return "~" + std::to_string(propsition_);
    }
  }
   
 private:
  const Proposition propsition_;
  bool positive_;
};

}  // namespace tiny_sat

#endif
