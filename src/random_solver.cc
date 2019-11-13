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
  int cur_index = sats_.size() - 1;
  bool ret = false;
  // Assign
  // Skip first assign
  if (prop != 0) {
#if 0
    std::cout << "sats" << std::endl;
#endif
    auto eval = cnf.eval(assign, prop, determined_clause_index_, sats_);
    cur_index = static_cast<int>(sats_.size()) - 1;
    if (eval == EVAL_UNSAT) {
      goto cleanup;
    }
  }

  // Cut determined clauses
  for (int i = prev_index; i <= cur_index; ++i) {
#if 0
    std::cout << "determine " << sats_[i] << std::endl;
#endif
    determined_clause_index_[sats_[i]] = true;
    ++determined_clauses_;
  }
  // If no more undetermined clause
  if (determined_clauses_ == cnf.size()) {
    ret = true;
    goto cleanup;
  }

  // Next turn
  prop = this->choose(cnf, assign);
  assign.assign(prop, EVAL_SAT);
  if (solve_impl(cnf, assign, prop)) {
    ret = true;
    goto cleanup;
  }
  assign.assign(prop, EVAL_UNSAT);
  ret = solve_impl(cnf, assign, prop);
  
  // Restore
  assign.remove(prop);
#if 0
  std::cout << "props" << std::endl;
#endif
  props_.push_back(prop);

cleanup:
  // Restore cut clauses
  for (int i = cur_index; i >= prev_index; --i) {
    if (determined_clause_index_[sats_[i]] == true) {
#if 0
      std::cout << "restore " << sats_[i] << std::endl;
#endif
      determined_clause_index_[sats_[i]] = false;
      --determined_clauses_;
    }
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
