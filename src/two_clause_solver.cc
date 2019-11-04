#include "solver.h"

namespace tiny_sat {

Proposition TwoClauseSolver::choose(const CNF &cnf, Assignment &assign) {
  return Proposition(0);
}

bool TwoClauseSolver::solve(const CNF &cnf, Assignment &assign) {
  return false;
}

}  // namespace tiny_sat
