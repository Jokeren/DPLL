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

  ~DIMACS() {
    if (input_.is_open()) {
      input_.close();
    }
  }
   
 private:
  std::ifstream input_;
};

}  // namespace tiny_sat

#endif
