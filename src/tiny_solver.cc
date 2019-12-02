#include "solver.h"

#include <iostream>

#define TINY_SOLVER_DEBUG 1
#define POLARITY_DEBUG 0

namespace tiny_sat {

void TinySolver::init(const CNF &cnf) {
  this->db_.init(cnf);
  this->pows_.resize(16);
  for (int i = 0; i < this->pows_.size(); ++i) {
    this->pows_[i] = pow(2, -i);
  }
  Proposition max_prop = 0;
  for (auto iter = this->db_.props().begin(); iter != this->db_.props().end(); ++iter) {
    max_prop = std::max(max_prop, *iter);
  }
  prop_scores_sat_.resize(max_prop + 1);
  prop_scores_unsat_.resize(max_prop + 1);
}


Proposition TinySolver::choose_unit(Assignment &assign, Evaluation &eval) {
  Proposition prop = 0;
  double max_score = 0.0;
  prop_scores_sat_.fill(0.0);
  prop_scores_unsat_.fill(0.0);

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
      if (liter_undecide->positive()) {
        prop_scores_sat_[liter_undecide->prop()] += this->pows_[undecided];
        if (prop_scores_unsat_[liter_undecide->prop()] != 0.0) {
          // resolution rule
          eval = EVAL_UNDECIDED;
          // prop cannot be zero
          return prop;
        }
      } else {
        prop_scores_unsat_[liter_undecide->prop()] += this->pows_[undecided];
        if (prop_scores_sat_[liter_undecide->prop()] != 0.0) {
          // resolution rule
          eval = EVAL_UNDECIDED;
          // prop cannot be zero
          return prop;
        }
      }
      double score = prop_scores_sat_[liter_undecide->prop()] +
        prop_scores_unsat_[liter_undecide->prop()];
      if (score > max_score) {
        // Ensure at least one proposition is selected
        prop = liter_undecide->prop();
        max_score = score;
      }
    }
  }

  if (prop != 0) {
    if (TINY_SOLVER_DEBUG) {
      std::cout << "Choose: " << prop << ", Positive score: " <<
        prop_scores_sat_[prop] << ", Negative score: " << prop_scores_unsat_[prop] << std::endl;
    }
    // Evaluate frequent one first
    if (prop_scores_sat_[prop] > prop_scores_unsat_[prop]) {
      eval = EVAL_SAT;
    } else {
      eval = EVAL_UNSAT;
    }
  }

  return prop;
}


Proposition TinySolver::choose_split(Assignment &assign,
  Evaluation &eval_first, Evaluation &eval_second) {
  Proposition prop = 0;
  double max_score = 0.0;
  prop_scores_sat_.fill(0.0);
  prop_scores_unsat_.fill(0.0);

  // non-unit 
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
        if (undecided <= this->pows_.size()) {
          if (liter->positive()) {
            prop_scores_sat_[liter->prop()] += this->pows_[undecided];
          } else {
            prop_scores_unsat_[liter->prop()] += this->pows_[undecided];
          }
        } else {
          if (liter->positive()) {
            prop_scores_sat_[liter->prop()] += pow(2, -undecided);
          } else {
            prop_scores_unsat_[liter->prop()] += pow(2, -undecided);
          }
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

  max_score = 0.0;
  if (POLARITY_DEBUG) {
    // Polarity does not work well
    // Choose polarity
    for (auto iter = db_.props().begin(); iter != db_.props().end(); ++iter) {
      auto p = *iter;
      if (prop_scores_sat_[p] == 0.0 && prop_scores_unsat_[p] != 0.0) {
        if (prop_scores_unsat_[p] > max_score) {
          // Ensure at least one proposition is selected
          prop = p;
          max_score = prop_scores_unsat_[p];
        }
      } else if (prop_scores_sat_[p] != 0.0 && prop_scores_unsat_[p] == 0.0) {
        if (prop_scores_sat_[p] > max_score) {
          // Ensure at least one proposition is selected
          prop = p;
          max_score = prop_scores_sat_[p];
        }
      }
    }
  }

  if (max_score == 0.0) {
    // Evaluate frequent one first
    if (prop_scores_sat_[prop] > prop_scores_unsat_[prop]) {
      eval_first = EVAL_SAT;
      eval_second = EVAL_UNSAT;
    } else {
      eval_first = EVAL_UNSAT;
      eval_second = EVAL_SAT;
    }
  } else {
    // Do not execute another branch
    if (prop_scores_sat_[prop] > prop_scores_unsat_[prop]) {
      eval_first = EVAL_SAT;
      eval_second = EVAL_UNDECIDED;
    } else {
      eval_first = EVAL_UNSAT;
      eval_second = EVAL_UNDECIDED;
    }
  }

  if (TINY_SOLVER_DEBUG) {
    std::cout << "Choose: " << prop << ", Positive score: " <<
      prop_scores_sat_[prop] << ", Negative score: " << prop_scores_unsat_[prop] << std::endl;
  }

  return prop;
}

}  // namespace tiny_sat
