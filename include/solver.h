#ifndef TINY_SAT_SOLVER_H
#define TINY_SAT_SOLVER_H

#include <queue>
#include <vector>
#include <random>
#include <set>
#include <unordered_map>
#include <unordered_set>

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
  virtual void init(const CNF &cnf) = 0;

  virtual Proposition choose_unit(Assignment &assign, Evaluation &eval) = 0;

  virtual Proposition choose_split(Assignment &assign,
    Evaluation &eval_first, Evaluation &eval_second) = 0;
 
 protected:
  SolverType type_;
  Database db_;
  std::mt19937 generator_;
};


class RandomSolver : public Solver {
 public:
  RandomSolver() : Solver(SOLVER_RANDOM) {}
 
 protected:
  virtual void init(const CNF &cnf);

  virtual Proposition choose_unit(Assignment &assign, Evaluation &eval);

  virtual Proposition choose_split(Assignment &assign,
    Evaluation &eval_first, Evaluation &eval_second);

 private:
  std::set<std::pair<Proposition, bool>> unit_candidates_;
};


class TwoClauseSolver : public Solver {
 public:
  TwoClauseSolver() : Solver(SOLVER_TWO_CLAUSE) {}

 protected:
  virtual void init(const CNF &cnf);

  virtual Proposition choose_unit(Assignment &assign, Evaluation &eval);

  virtual Proposition choose_split(Assignment &assign,
    Evaluation &eval_first, Evaluation &eval_second);

 private:
  std::unordered_map<Proposition, size_t> two_clause_props_;
  std::set<std::pair<Proposition, bool>> unit_candidates_;
  std::vector<Proposition> two_clause_candidates_;
};


class TinySolver : public Solver {
 public:
  TinySolver() : Solver(SOLVER_TINY) {}

 protected:
  virtual void init(const CNF &cnf);

  virtual Proposition choose_unit(Assignment &assign, Evaluation &eval);

  virtual Proposition choose_split(Assignment &assign,
    Evaluation &eval_first, Evaluation &eval_second);

 private:
  Path<double> prop_scores_sat_;
  Path<double> prop_scores_unsat_;
  Path<double> pows_;
};

}  // namespace tiny_sat

#endif
