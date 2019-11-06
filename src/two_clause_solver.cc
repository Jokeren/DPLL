#include "solver.h"

#include <map>

namespace tiny_sat {

Proposition TwoClauseSolver::choose(const CNF &cnf, Assignment &assign) {
  std::map<Proposition, int> prop_clauses;
  Proposition prop;
  int max_clause = 0;
  for (auto citer = cnf.begin(); citer != cnf.end(); ++citer) {
    int sum = 0;
    for (auto liter = citer->begin(); liter != citer->end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        prop = liter->prop();
        ++sum;
      }
    }
    // find a two-clause
    if (sum == 2) {
      for (auto liter = citer->begin(); liter != citer->end(); ++liter) {
        if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
          prop_clauses[liter->prop()]++;
          max_clause = std::max(max_clause, prop_clauses[liter->prop()]);
        }
      }
    }
  }

  std::vector<Proposition> candidates;
  for (auto &iter : prop_clauses) {
    if (iter.second == max_clause) {
      candidates.push_back(iter.first);
    }
  }

  if (candidates.size() != 0) {
    std::uniform_int_distribution<size_t> distribution(0, candidates.size() - 1);
    auto index = distribution(generator_);
    prop = candidates[index];
  }

  return prop;
}


bool TwoClauseSolver::solve_impl(const CNF &cnf, Assignment &assign) {
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
  return ret;
}


bool TwoClauseSolver::solve(const CNF &cnf, Assignment &assign) {
  return solve_impl(cnf, assign);
}

}  // namespace tiny_sat
