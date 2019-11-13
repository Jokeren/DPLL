#ifndef TINY_SAT_DATABASE_H
#define TINY_SAT_DATABASE_H

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "proposition.h"
#include "clause.h"
#include "cnf.h"

namespace tiny_sat {

class Database {
 public:
  Database() {}

  Evaluation eval(Proposition prop, Assignment &assignment);

  void rollback();

  void init(const CNF &cnf);


  bool satisfy() const {
    return clauses_.size() == 0;
  }

  const std::unordered_set<Proposition> &props() const {
    return props_;
  }

  const std::unordered_set<const Clause *> &clauses() const {
    return clauses_;
  }

 private:
  std::unordered_set<Proposition> props_;
  std::unordered_set<const Clause *> clauses_;
  std::unordered_map<Proposition, std::vector<const Clause *>> prop_clauses_;
  std::stack<std::pair<Proposition, std::vector<const Clause *>>> free_clauses_;
};

}  // namespace tiny_sat

#endif 
