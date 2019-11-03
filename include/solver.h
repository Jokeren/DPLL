#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include "cnf.h"

namespace tiny_sat {

class Solver {
 public:
  Solver() {}

  virtual bool solve(const CNF &cnf) = 0;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() {}

  virtual bool solve(const CNF &cnf);
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() {}

  virtual bool solve(const CNF &cnf);
};


class TinySolver : public Solver {
 public:
  TinySolver() {}

  virtual bool solve(const CNF &cnf);
};

}  // namespace tiny_sat

#endif
