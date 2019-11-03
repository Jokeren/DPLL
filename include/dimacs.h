#ifndef TINY_SAT_DIMACS_H
#define TINY_SAT_DIMACS_H

#include "cnf.h"

namespace tiny_sat {

class DIMACS {
 public:
  DIMACS() {}

  bool open(const std::string &file_path);

  void read(CNF &cnf);
   
 private:
  std::string file_path_;
};

}  // namespace tiny_sat

#endif
