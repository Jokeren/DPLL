#ifndef TINY_SAT_DIMACS_H
#define TINY_SAT_DIMACS_H

#include <fstream>

#include "cnf.h"

namespace tiny_sat {

class DIMACS {
 public:
  DIMACS() {}

  bool open(const std::string &file_path);

  void read(CNF &cnf);

  void generate(int propositions, int clauses, int literals);

  ~DIMACS() {
    if (file_.is_open()) {
      file_.close();
    }
  }
   
 private:
  std::ifstream file_;
};

}  // namespace tiny_sat

#endif
