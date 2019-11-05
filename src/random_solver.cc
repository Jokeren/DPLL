#include "solver.h"

#include <random>

namespace tiny_sat {

Proposition RandomSolver::choose(const CNF &cnf, Assignment &assign) {
  std::uniform_int_distribution<size_t> distribution(0, props_.size() - 1);
  auto index = distribution(generator_);
  auto prop = props_[index];
  props_.erase(props_.begin() + index);

  return prop;
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
  bool ret = solve_impl(cnf, assign);
  assign.remove(prop);
  props_.push_back(prop);
  return ret;
}


bool RandomSolver::solve(const CNF &cnf, Assignment &assign) {
  props_ = cnf.props();
  return solve_impl(cnf, assign);
}

}  // namespace tiny_sat
