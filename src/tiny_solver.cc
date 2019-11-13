#include "solver.h"
#include <signal.h>

#include <algorithm>
#include <map>

namespace tiny_sat {

void TinySolver::init(const CNF &cnf) {
  this->db_.init(cnf);
}


Proposition TinySolver::choose(Assignment &assign) {
  return 0;
}

}  // namespace tiny_sat
