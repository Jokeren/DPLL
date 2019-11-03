#ifndef TINY_SAT_CNF_H
#define TINY_SAT_CNF_H

#include "clause.h"

namespace tiny_sat {

class CNF {
 public:
  CNF() {}

  void add(Clause &clause);

  std::vector<Clause>::iterator begin() {
    return clauses_.begin();
  }

  std::vector<Clause>::iterator end() {
    return clauses_.end();
  }
   
 private:
  std::vector<Clause> clauses_;
};

}  // namespace tiny_sat

#endif
