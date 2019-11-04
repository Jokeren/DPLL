#include "solver.h"

namespace tiny_sat {

Proposition TinySolver::choose(const CNF &cnf, Assignment &assign) {
  return Proposition(0);
}


bool TinySolver::solve(const CNF &cnf, Assignment &assign) {
  return false;
}

}  // namespace tiny_sat
