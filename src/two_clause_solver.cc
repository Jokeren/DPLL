#include "database.h"
#include "solver.h"

namespace tiny_sat {

void TwoClauseSolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition TwoClauseSolver::choose_unit(Assignment &assign, Evaluation &eval) {
  Proposition prop = 0;
  unit_candidates_.clear();
  // propagate unit literal first
  for (auto iter = db_.clauses().begin(); iter != db_.clauses().end(); ++iter) {
    auto *clause = *iter;
    int undecided = 0;
    auto liter_undecide = clause->begin();
    for (auto liter = clause->begin(); liter != clause->end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        liter_undecide = liter;
        ++undecided;
      }
    }
    if (undecided == 1) {
      unit_candidates_.insert(std::pair<Proposition, bool>(
        liter_undecide->prop(), liter_undecide->positive()));
    }
  }

  if (unit_candidates_.size() != 0) {
    std::uniform_int_distribution<size_t> dist_props(0, unit_candidates_.size() - 1);
    size_t index = dist_props(generator_);
    auto it = std::next(std::begin(unit_candidates_), index);
    prop = it->first;

    if (it->second == true) {
      eval = EVAL_SAT;
    } else {
      eval = EVAL_UNSAT;
    }
  }

  return prop;
}


Proposition TwoClauseSolver::choose_split(Assignment &assign,
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

  two_clause_candidates_.clear();
  for (auto &iter : two_clause_props_) {
    if (iter.second == max_clause) {
      two_clause_candidates_.push_back(iter.first);
    }
  }

  if (two_clause_candidates_.size() != 0) {
    std::uniform_int_distribution<size_t> distribution(0,
      two_clause_candidates_.size() - 1);
    auto index = distribution(generator_);
    prop = two_clause_candidates_[index];
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
