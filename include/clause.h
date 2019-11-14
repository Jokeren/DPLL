#ifndef TINY_SAT_CLAUSE_H
#define TINY_SAT_CLAUSE_H

#include <map>
#include <vector>

#include "common.h"
#include "literal.h"

namespace tiny_sat {

class Clause {
 public:
  Clause(size_t index = 0) : index_(index) {}

  size_t index() const {
    return index_;
  }

  void add(const Literal &literal) {
    literals_.push_back(literal);
  }

  void add(const Literal &&literal) {
    literals_.emplace_back(literal);
  }

  std::vector<Literal>::iterator begin() {
    return literals_.begin();
  }

  std::vector<Literal>::iterator end() {
    return literals_.end();
  }

  std::vector<Literal>::const_iterator begin() const {
    return literals_.begin();
  }

  std::vector<Literal>::const_iterator end() const {
    return literals_.end();
  }

  Evaluation eval(const Assignment &assignment) const {
    bool undecided = false;
    for (auto iter = literals_.begin(); iter != literals_.end(); ++iter) {
      auto ret = iter->eval(assignment);
      if (ret == EVAL_SAT) {
        return ret;
      } else if (ret == EVAL_UNDECIDED) {
        undecided = true;
      }
    }
    return undecided ? EVAL_UNDECIDED : EVAL_UNSAT;
  }

  std::string to_string() const {
    std::stringstream ss;
    for (auto &literal : literals_) {
      ss << literal.to_string() << " ";
    }
    return ss.str();
  }
   
 private:
  size_t index_;

  std::vector<Literal> literals_;
};

}  // namespace tiny_sat

#endif
