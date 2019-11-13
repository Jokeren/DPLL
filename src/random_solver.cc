#include "solver.h"

#include <random>
#include <iostream>

namespace tiny_sat {

void RandomSolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition RandomSolver::choose(Assignment &assign) {
  std::uniform_int_distribution<size_t> distribution(0, db_.props().size() - 1);
  size_t index = distribution(generator_);
  auto it = std::next(std::begin(db_.props()), index);
  auto prop = *it;

  return prop;
}

}  // namespace tiny_sat
