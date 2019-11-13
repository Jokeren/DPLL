#include "solver.h"

#include <map>

namespace tiny_sat {

Proposition TwoClauseSolver::choose(const CNF &cnf, Assignment &assign) {
  Proposition prop;
  prop_clauses_.clear();
  size_t max_clause = 0;
  for (auto index : two_sats_) {
    auto &clause = cnf.get(index);
    for (auto liter = clause.begin(); liter != clause.end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        prop_clauses_[liter->prop()]++;
        max_clause = std::max(max_clause, prop_clauses_[liter->prop()]);
      }
    }
  }

  candidates_.reset();
  for (auto &iter : prop_clauses_) {
    if (iter.second == max_clause) {
      candidates_.push_back(iter.first);
    }
  }

  if (candidates_.size() != 0) {
    std::uniform_int_distribution<size_t> distribution(0, candidates_.size() - 1);
    auto index = distribution(generator_);
    prop = candidates_[index];
  } else {
    prop = *(props_.begin());
  }
  props_.erase(prop);

  return prop;
}


bool TwoClauseSolver::solve_impl(const CNF &cnf, Assignment &assign, Proposition prop) {
  int prev_index = sats_.size();
  int cur_index = sats_.size() - 1;
  std::set<size_t> one_sats;
  std::set<size_t> two_sats;

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

    std::vector<size_t> two_sat_clauses;
    cnf.get_clauses(prop, two_sat_clauses);
    for (auto index : two_sat_clauses) {
      auto &clause = cnf.get(index);
      size_t sum = 0;
      for (auto liter = clause.begin(); liter != clause.end(); ++liter) {
        if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
          ++sum;
        }
      }
      if (sum == 1) {
        one_sats.insert(clause.index());
        two_sats_.erase(clause.index());
      } else if (sum == 2) {
        two_sats.insert(clause.index());
        two_sats_.insert(clause.index());
      }
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
  props_.insert(prop);

cleanup:
  for (auto index : two_sats) {
    two_sats_.erase(index);
  }
  for (auto index : one_sats) {
    two_sats_.insert(index);
  }
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


bool TwoClauseSolver::solve(const CNF &cnf, Assignment &assign) {
  auto props = cnf.props();
  for (auto &prop : props) {
    props_.insert(prop);
  }
  determined_clause_index_.resize(cnf.size() + 1);
  determined_clause_index_.fill(false);
  determined_clauses_ = 0;
  Proposition prop(0);
  return solve_impl(cnf, assign, prop);
}

}  // namespace tiny_sat
