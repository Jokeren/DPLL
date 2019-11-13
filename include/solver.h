#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include <queue>
#include <vector>
#include <random>
#include <set>

#include "assignment.h"
#include "cnf.h"
#include "database.h"
#include "path.h"


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

  virtual bool solve(const CNF &cnf, Assignment &assign);

  virtual bool solve_impl(Assignment &assign, Proposition prop);

  virtual SolverType type() {
    return this->type_;
  }

 protected:
  virtual Proposition choose(Assignment &assign) = 0;
 
 protected:
  SolverType type_;
  Database db_;
  std::mt19937 generator_;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() : Solver(SOLVER_RANDOM) {}
 
 protected:
  virtual Proposition choose(Assignment &assign);
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() : Solver(SOLVER_TWO_CLAUSE) {}

 protected:
  virtual Proposition choose(Assignment &assign);

 private:
  std::map<Proposition, size_t> two_clause_props_;
  std::vector<Proposition> candidates_;
};


class TinySolver : public Solver {
 public:
  TinySolver() : Solver(SOLVER_TINY) {}

 protected:
  virtual Proposition choose(Assignment &assign);
};

}  // namespace tiny_sat

#endif
