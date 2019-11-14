#include "database.h"
#include "solver.h"

namespace tiny_sat {

bool Solver::solve_impl(Assignment &assign, Proposition prop) {
  // Skip first assign
  if (prop != 0) {
    // Cut determined clauses
    auto eval = db_.eval(prop, assign);
    if (eval == EVAL_UNSAT) {
      db_.rollback();
      return false;
    }
  }

  // If no more undetermined clause
  if (db_.satisfy()) {
    db_.rollback();
    return true;
  }

  // Next turn
  Evaluation eval_first, eval_second;
  prop = this->choose(assign, eval_first, eval_second);

  assign.assign(prop, eval_first);
  if (solve_impl(assign, prop)) {
    db_.rollback();
    return true;
  }
  assign.assign(prop, eval_second);
  if (solve_impl(assign, prop)) {
    db_.rollback();
    return true;
  }
  assign.remove(prop);
  db_.rollback();
  return false;
}


bool Solver::solve(const CNF &cnf, Assignment &assign) {
  this->init(cnf);
  Proposition prop = 0;
  return solve_impl(assign, prop);
}

}  // namespace tiny_sat
