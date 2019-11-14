#ifndef TINY_SAT_CNF_H
#define TINY_SAT_CNF_H

#include "clause.h"

namespace tiny_sat {

class CNF {
 public:
  CNF() {}

  size_t size() const {
    return this->clauses_.size();
  }

  const Clause &get(size_t index) const {
    return this->clauses_[index];
  }

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
  }

  void add(const Clause &&clause) {
    clauses_.emplace_back(clause);
  }

  Evaluation eval(const Assignment &assignment) const {
    bool undecided = false;
    for (auto iter = clauses_.begin(); iter != clauses_.end(); ++iter) {
      auto ret = iter->eval(assignment);
      if (ret == EVAL_UNSAT) {
        return ret;
      } else if (ret == EVAL_UNDECIDED) {
        undecided = true;
      }
    }
    return undecided ? EVAL_UNDECIDED : EVAL_SAT;
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
};

}  // namespace tiny_sat

#endif
