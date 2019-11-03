#ifndef TINY_SAT_CLAUSE_H
#define TINY_SAT_CLAUSE_H

#include <vector>

#include "literal.h"

namespace tiny_sat {

class Clause {
 public:
  Clause() {}

  void add(Literal &literal);

  std::vector<Literal>::iterator begin() {
    return literals_.begin();
  }

  std::vector<Literal>::iterator end() {
    return literals_.end();
  }
   
 private:
  std::vector<Literal> literals_;
};

}  // namespace tiny_sat

#endif
