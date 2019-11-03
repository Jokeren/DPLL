#ifndef TINY_SAT_LITERAL_H
#define TINY_SAT_LITERAL_H

#include "proposition.h"

namespace tiny_sat {

class Literal {
 public:
  Literal() : propsition_(Proposition(0)), positive_(true) {}

  Literal(const Proposition &prop, bool positive = true) :
    propsition_(prop), positive_(positive) {}

  bool positive() {
    // Leave zero as an undefined proposition marker
    assert(this->propsition_->id() != 0);
    return positive_;
  }

  size_t id() : {
    // Leave zero as an undefined proposition marker
    assert(this->propsition_->id() != 0);
    return this->propsition_->id();
  }
   
 private:
  Proposition propsition_;
  bool negative_;
};

}  // namespace tiny_sat

#endif
