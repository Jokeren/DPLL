#include "database.h"


namespace tiny_sat {

void Database::init(const CNF &cnf) {
  clauses_.clear();
  props_.clear();
  prop_clauses_.clear();
  while (!free_clauses_.empty()) {
    free_clauses_.pop();
  }

  for (auto citer = cnf.begin(); citer < cnf.end(); ++citer) {
    auto *clause = &(*citer);
    clauses_.insert(clause);
    for (auto iter = citer->begin(); iter != citer->end(); ++iter) {
      props_.insert(iter->prop());
      prop_clauses_[iter->prop()].emplace_back(clause);
    }
  }
}


Evaluation Database::eval(Proposition prop, Assignment &assignment) {
  auto pop_iter = prop_clauses_.find(prop);
  if (pop_iter == prop_clauses_.end()) {
    // not find a prop, return satisfy
    return EVAL_SAT;
  }
  auto &clauses = pop_iter->second;

  bool undecided = false;
  bool unsat = false;
  std::pair<Proposition, std::vector<const Clause *>> free_clauses;
  free_clauses.first = prop;
  for (auto iter = clauses.begin(); iter != clauses.end(); ++iter) {
    auto *clause = *iter;
    auto ret = clause->eval(assignment);
    bool active = true;
    if (clauses_.find(clause) != clauses_.end()) {
      if (ret == EVAL_UNSAT) {
        unsat = true;
        active = false;
      } else if (ret == EVAL_UNDECIDED) {
        undecided = true;
      } else {
        active = false;
      }
      // move pointer
      if (!active) {
        clauses_.erase(clause);
        free_clauses.second.push_back(clause);
      }
    }
  }
  // commit
  props_.erase(prop);
  free_clauses_.emplace(free_clauses);
  return unsat ? EVAL_UNSAT : (undecided ? EVAL_UNDECIDED : EVAL_SAT);
}


void Database::rollback() {
  if (!free_clauses_.empty()) {
    auto p = free_clauses_.top();
    free_clauses_.pop();
    auto prop = p.first;
    auto &clauses = p.second;
    for (auto *clause : clauses) {
      clauses_.insert(clause);
    }
    props_.insert(prop);
  }
}


}  // namespace tiny_sat
