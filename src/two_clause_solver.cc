#include "database.h"
#include "solver.h"

#include <map>

namespace tiny_sat {

void TwoClauseSolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition TwoClauseSolver::choose(Assignment &assign,
  Evaluation &eval_first, Evaluation &eval_second) {
  Proposition prop = 0;
  size_t max_clause = 0;
  two_clause_props_.clear();
  for (auto iter = db_.clauses().begin(); iter != db_.clauses().end(); ++iter) {
    auto *clause = *iter;
    int undecided = 0;
    for (auto liter = clause->begin(); liter != clause->end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        // Ensure at least one proposition is selected
        prop = liter->prop();
        ++undecided;
      }
    }
    if (undecided == 2) {
      for (auto liter = clause->begin(); liter != clause->end(); ++liter) {
        if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
          two_clause_props_[liter->prop()]++;
          max_clause = std::max(max_clause, two_clause_props_[liter->prop()]);
        }
      }
    }
  }

  candidates_.clear();
  for (auto &iter : two_clause_props_) {
    if (iter.second == max_clause) {
      candidates_.push_back(iter.first);
    }
  }

  if (candidates_.size() != 0) {
    std::uniform_int_distribution<size_t> distribution(0, candidates_.size() - 1);
    auto index = distribution(generator_);
    prop = candidates_[index];
  }

  // Random choose true and false order
  std::uniform_int_distribution<int> dist_evals(0, 1);
  if (dist_evals(generator_) == 0) {
    eval_first = EVAL_SAT;
    eval_second = EVAL_UNSAT;
  } else {
    eval_first = EVAL_UNSAT;
    eval_second = EVAL_SAT;
  }

  return prop;
}


}  // namespace tiny_sat
