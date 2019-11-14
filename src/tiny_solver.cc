#include "solver.h"
#include <signal.h>

#include <algorithm>
#include <map>

#define TINY_SOLVER_DEBUG 0

namespace tiny_sat {

void TinySolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition TinySolver::choose(Assignment &assign,
  Evaluation &eval_first, Evaluation &eval_second) {
  Proposition prop = 0;
  double max_score = 0.0;
  prop_scores_sat_.clear();
  prop_scores_unsat_.clear();
  for (auto iter = db_.clauses().begin(); iter != db_.clauses().end(); ++iter) {
    auto *clause = *iter;
    int undecided = 0;
    for (auto liter = clause->begin(); liter != clause->end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        ++undecided;
      }
    }
    for (auto liter = clause->begin(); liter != clause->end(); ++liter) {
      if (assign.get(liter->prop()) == EVAL_UNDECIDED) {
        if (liter->positive()) {
          prop_scores_sat_[liter->prop()] += pow(2, -undecided);
        } else {
          prop_scores_unsat_[liter->prop()] += pow(2, -undecided);
        }
        double score = prop_scores_sat_[liter->prop()] +
          prop_scores_unsat_[liter->prop()];
        if (score > max_score) {
          // Ensure at least one proposition is selected
          prop = liter->prop();
          max_score = score;
        }
      }
    }
  }

  if (TINY_SOLVER_DEBUG) {
    std::cout << "Choose: " << prop << ", Positive score: " <<
      prop_scores_sat_[prop] << ", Negative score: " << prop_scores_unsat_[prop] << std::endl;
  }

  // Evaluate frequent one first
  if (prop_scores_sat_[prop] > prop_scores_unsat_[prop]) {
    eval_first = EVAL_SAT;
    eval_second = EVAL_UNSAT;
  } else {
    eval_first = EVAL_UNSAT;
    eval_second = EVAL_SAT;
  }

  return prop;
}

}  // namespace tiny_sat
