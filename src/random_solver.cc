#include "solver.h"

namespace tiny_sat {

Proposition RandomSolver::choose(const CNF &cnf, Assignment &assign) {
  return Proposition(0);
}


bool RandomSolver::solve_impl(const CNF &cnf, Assignment &assign) {
  auto eval = cnf.eval(assign);
  if (eval == EVAL_SAT) {
    return true;
  } else if (eval == EVAL_UNSAT) {
    return false;
  }
  auto prop = this->choose(cnf, assign);
  assign.assign(prop, true);
  if (solve_impl(cnf, assign)) {
    return true;
  }
  assign.assign(prop, false);
  return solve_impl(cnf, assign);
}


bool RandomSolver::solve(const CNF &cnf, Assignment &assign) {
  std::vector<Proposition> props = cnf.props();
  return solve_impl(cnf, assign);
}

}  // namespace tiny_sat
