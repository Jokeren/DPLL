#ifndef TINY_SAT_CNF_H
#define TINY_SAT_CNF_H

#include "clause.h"

namespace tiny_sat {

class CNF {
 public:
  CNF() {}

  std::vector<Clause>::iterator begin() {
    return clauses_.begin();
  }

  std::vector<Clause>::iterator end() {
    return clauses_.end();
  }

  std::vector<Clause>::const_iterator begin() const {
    return clauses_.begin();
  }

  std::vector<Clause>::const_iterator end() const {
    return clauses_.end();
  }

  void add(const Clause &clause) {
    clauses_.push_back(clause);
    for (auto iter = clause.begin(); iter != clause.end(); ++iter) {
      props_[iter->prop()].push_back(clauses_.size() - 1);
    }
  }

  void add(const Clause &&clause) {
    clauses_.emplace_back(clause);
    for (auto iter = clause.begin(); iter != clause.end(); ++iter) {
      props_[iter->prop()].push_back(clauses_.size() - 1);
    }
  }

  Evaluation eval(const Assignment &assignment) const {
    bool undecided = false;
    for (auto iter = clauses_.begin(); iter != clauses_.end(); ++iter) {
      auto ret = iter->eval(assignment);
      if (ret == EVAL_SAT) {
        return ret;
      } else if (ret == EVAL_UNDECIDED) {
        undecided = true;
      }
    }
    return undecided ? EVAL_UNDECIDED : EVAL_UNSAT;
  }

  std::vector<Proposition> props() const {
    std::vector<Proposition> prop_vec;
    for (auto iter : props_) {
      prop_vec.push_back(iter.first);
    }
    return prop_vec;
  }

  std::string to_string() const {
    std::stringstream ss;
    for (auto &clause : clauses_) {
      ss << clause.to_string() << std::endl;
    }
    return ss.str();
  }
   
 private:
  std::vector<Clause> clauses_;
  // Proposition->clause index
  std::map<Proposition, std::vector<size_t> > props_;
};

}  // namespace tiny_sat

#endif
