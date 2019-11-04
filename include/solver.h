#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include "cnf.h"
#include "assignment.h"


namespace tiny_sat {

enum SolverType {
  RANDOM = 0,
  TWO_CLAUSE = 1,
  TINY = 2,
  SOLVER_TYPE_COUNT = 3
};


class Solver {
 public:
  explicit Solver(SolverType type) : type_(type) {}

  virtual bool solve(const CNF &cnf, Assignment &assign) = 0;

  virtual SolverType type() {
    return this->type_;
  }
 
 protected:
  SolverType type_;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() : Solver(RANDOM) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() : Solver(TWO_CLAUSE) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
};


class TinySolver : public Solver {
 public:
  TinySolver() : Solver(TINY) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
};

}  // namespace tiny_sat

#endif
