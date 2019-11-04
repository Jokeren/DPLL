#ifndef TINY_SAT_CLAUSE_H
#define TINY_SAT_CLAUSE_H

#include <vector>

#include "literal.h"

namespace tiny_sat {

class Clause {
 public:
  Clause() {}

  void add(const Literal &literal) {
    literals_.push_back(literal);
    for (auto iter = literals_.begin(); iter != literals_.end(); ++iter) {
      props_[iter->prop()].push_back(literals_.size() - 1);
    }
  }

  void add(const Literal &&literal) {
    literals_.emplace_back(literal);
    for (auto iter = literals_.begin(); iter != literals_.end(); ++iter) {
      props_[iter->prop()].push_back(literals_.size() - 1);
    }
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

  bool eval(const Assignment &assignment) const {
    for (auto iter = literals_.begin(); iter != literals_.end(); ++iter) {
      if (iter->eval(assignment)) {
        return true;
      }
    }
    return false;
  }

  std::string to_string() const {
    std::stringstream ss;
    for (auto &literal : literals_) {
      ss << literal.to_string() << " ";
    }
    return ss.str();
  }
   
 private:
  std::vector<Literal> literals_;
  // Proposition->literal index
  std::map<Proposition, std::vector<size_t> > props_;
};

}  // namespace tiny_sat

#endif
