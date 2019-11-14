#include "solver.h"

#include <random>

namespace tiny_sat {

void RandomSolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition RandomSolver::choose(Assignment &assign,
  Evaluation &eval_first, Evaluation &eval_second) {
  std::uniform_int_distribution<size_t> dist_props(0, db_.props().size() - 1);
  size_t index = dist_props(generator_);
  auto it = std::next(std::begin(db_.props()), index);
  auto prop = *it;

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
