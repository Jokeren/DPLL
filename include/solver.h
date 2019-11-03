#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include "cnf.h"

namespace tiny_sat {

class Solver {
 public:
  enum SolverType {
    RANDOM = 0,
    TWO_CLAUSE = 1,
    TINY = 2,
    SOLVER_TYPE_COUNT = 3
  };

 public:
  Solver() {}

  virtual bool solve(const CNF &cnf) = 0;

  virtual SolverType type() {
    return this->type_;
  }
 
 protected:
  SolverType type_;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() : type_(RANDOM) {}

  virtual bool solve(const CNF &cnf);
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() : type_(TWO_CLAUSE) {}

  virtual bool solve(const CNF &cnf);
};


class TinySolver : public Solver {
 public:
  TinySolver() : type_(TINY) {}

  virtual bool solve(const CNF &cnf);
};

}  // namespace tiny_sat

#endif
