#include "solver.h"

#include <random>
#include <iostream>

namespace tiny_sat {

Proposition RandomSolver::choose(const CNF &cnf, Assignment &assign) {
  std::uniform_int_distribution<size_t> distribution(0, props_.size() - 1);
  auto index = distribution(generator_);
  auto prop = props_.get(index);
  props_.remove(index);

  return prop;
}


bool RandomSolver::solve_impl(const CNF &cnf, Assignment &assign, Proposition prop) {
  int prev_index = sats_.size();
  // Assign
  // Skip first assign
  if (prop != 0) {
    auto eval = cnf.eval(assign, prop, determined_clause_index_, sats_);
    if (eval == EVAL_UNSAT) {
      return false;
    }
  }

  int cur_index = static_cast<int>(sats_.size()) - 1;
  std::cout << "prev " << prev_index << " cur " << cur_index << std::endl;
  // Cut determined clauses
  for (int i = prev_index; i <= cur_index; ++i) {
    determined_clause_index_[sats_[i]] = true;
    ++determined_clauses_;
  }
  // If no more undetermined clause
  if (determined_clauses_ == cnf.size()) {
    return true;
  }

  // Next turn
  prop = this->choose(cnf, assign);
  assign.assign(prop, true);
  if (solve_impl(cnf, assign, prop)) {
    return true;
  }
  assign.assign(prop, false);
  bool ret = solve_impl(cnf, assign, prop);
  
  // Restore
  assign.remove(prop);
  props_.push_back(prop);
  // Restore cut clauses
  for (int i = cur_index - 1; i >= prev_index; ++i) {
    determined_clause_index_[sats_[i]] = false;
    --determined_clauses_;
    sats_.pop_back();
  }
  return ret;
}


bool RandomSolver::solve(const CNF &cnf, Assignment &assign) {
  auto props = cnf.props();
  for (auto &prop : props) {
    props_.push_back(prop);
  }
  determined_clause_index_.resize(cnf.size() + 1);
  determined_clause_index_.fill(false);
  determined_clauses_ = 0;
  Proposition prop(0);
  return solve_impl(cnf, assign, prop);
}

}  // namespace tiny_sat
