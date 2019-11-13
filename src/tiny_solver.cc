#include "solver.h"
#include <signal.h>

#include <algorithm>
#include <map>

namespace tiny_sat {

Proposition TinySolver::choose(const CNF &cnf, Assignment &assign, std::vector<int> &frequency) {
  Proposition max_index = 0;
  int max_freq = 0;
  for (Proposition i = 0; i < frequency.size(); ++i) {
    if (max_freq < frequency[i]) {
      max_index = i;
      max_freq = frequency[i];
    }
  }
  frequency[max_index] = 0;
  return max_index;
}


bool TinySolver::solve_impl(const CNF &cnf, Assignment &assign, Proposition prop, std::vector<int> frequency) {
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
    auto &clause = cnf.get(sats_[i]); 
    for (auto iter = clause.begin(); iter != clause.end(); ++iter) {
      auto prop = iter->prop();
      if (frequency[prop] != 0) {
        --frequency[prop];
      }
    }
  }
  // If no more undetermined clause
  if (determined_clauses_ == cnf.size()) {
    ret = true;
    goto cleanup;
  }

  // Next turn
  prop = this->choose(cnf, assign, frequency);
  if (prop == 0) {
    raise(SIGABRT);
  }
  assign.assign(prop, EVAL_SAT);
  if (solve_impl(cnf, assign, prop, frequency)) {
    ret = true;
    goto cleanup;
  }
  assign.assign(prop, EVAL_UNSAT);
  ret = solve_impl(cnf, assign, prop, frequency);
  
  // Restore
  assign.remove(prop);
#if 0
  std::cout << "props" << std::endl;
#endif
  props_.insert(prop);

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

bool TinySolver::solve(const CNF &cnf, Assignment &assign) {
  auto props = cnf.props();
  for (auto &prop : props) {
    props_.insert(prop);
  }
  determined_clause_index_.resize(cnf.size() + 1);
  determined_clause_index_.fill(false);
  determined_clauses_ = 0;
  std::vector<int> frequency(props_.size() + 1);
  for (Proposition i = 1; i <= props.size(); ++i) {
    std::vector<size_t> clauses;
    cnf.get_clauses(i, clauses);
    int size = clauses.size();
    frequency[i] = size;
  }
  //std::sort(frequency.begin(), frequency.end());
  //for (auto freq : frequency) {
  //  std::cout << freq.prop << " : " << freq.freq << std::endl;
  //}
  for (size_t i = 0; i < frequency.size(); ++i) {
    std::cout << i << " : " << frequency[i] << std::endl;
  }
  Proposition prop(0);
  return solve_impl(cnf, assign, prop, frequency);
}

}  // namespace tiny_sat
