#ifndef TINY_SAT_LITERAL_H
#define TINY_SAT_LITERAL_H

#include "proposition.h"
#include "assignment.h"

#include <cassert>


namespace tiny_sat {

class Literal {
 public:
  Literal() : propsition_(Proposition(0)), positive_(true) {}

  Literal(const Proposition &prop, bool positive = true) :
    propsition_(prop), positive_(positive) {}

  bool positive() {
    // Leave zero as an undefined proposition marker
    assert(propsition_.id() != 0);
    return positive_;
  }

  bool eval(const Assignment &assignment) {
    if (assignment.get(propsition_)) {
      return positive_;
    }
    return !positive_;
  }

  size_t id() {
    // Leave zero as an undefined proposition marker
    assert(propsition_.id() != 0);
    return propsition_.id();
  }

  Proposition prop() {
    return propsition_;
  }
   
 private:
  Proposition propsition_;
  bool positive_;
};

}  // namespace tiny_sat

#endif
