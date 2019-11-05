#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include <queue>
#include <vector>
#include <random>

#include "cnf.h"
#include "assignment.h"


namespace tiny_sat {

enum SolverType {
  SOLVER_RANDOM = 0,
  SOLVER_TWO_CLAUSE = 1,
  SOLVER_TINY = 2,
  SOLVER_SOLVER_TYPE_COUNT = 3
};


class Solver {
 public:
  explicit Solver(SolverType type) : type_(type) {}

  virtual bool solve(const CNF &cnf, Assignment &assign) = 0;

  virtual SolverType type() {
    return this->type_;
  }
 
 protected:
  virtual Proposition choose(const CNF &cnf, Assignment &assign) = 0;

  SolverType type_;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() : Solver(SOLVER_RANDOM) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
 
 protected:
  virtual Proposition choose(const CNF &cnf, Assignment &assign);
 
 private:
  bool solve_impl(const CNF &cnf, Assignment &assign);

 private:
  std::mt19937 generator_;
  std::vector<Proposition> props_;
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() : Solver(SOLVER_TWO_CLAUSE) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
 
 protected:
  virtual Proposition choose(const CNF &cnf, Assignment &assign);
};


class TinySolver : public Solver {
 public:
  TinySolver() : Solver(SOLVER_TINY) {}

  virtual bool solve(const CNF &cnf, Assignment &assign);
 
 protected:
  virtual Proposition choose(const CNF &cnf, Assignment &assign);
};

}  // namespace tiny_sat

#endif
