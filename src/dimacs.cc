#include "dimacs.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <cstdlib>

#include "common.h"


namespace tiny_sat {

bool DIMACS::open(const std::string &file_path) {
  input_.open(file_path, std::ifstream::in);
  if (!input_.good()) {
    TINY_SAT_LOG_WARNING("File Reader", "open file error!");
    return false;
  }
  return true;
}


void DIMACS::read(CNF &cnf) {
  std::string buf;
  int props = 0;
  int clauses = 0;

  while (std::getline(input_, buf)) {
    size_t p = 0;
    while (p < buf.size() && buf[p] == ' ') {
      ++p;
    }

    if (p == buf.size()) {
      // Skip empty lines
      continue;
    } else if (buf[p] == 'c') {
      // Skip comment lines
      continue;
    } else if (buf[p] == 'p') {
      // A problem line
      std::stringstream ss(buf);
      std::string s;
      std::vector<std::string> args;
      
      while (std::getline(ss, s, ' ')) {
        args.push_back(s);
      }

      if (args.size() != 4 || args[1] != "cnf") {
        TINY_SAT_LOG_ERROR("File Reader", "problem line error!");
      }

      props = std::stoi(args[2]);
      clauses = std::stoi(args[3]);
    } else {
      // A clause line
      std::stringstream ss(buf);
      std::string s;
      std::vector<std::string> args;
      
      while (std::getline(ss, s, ' ')) {
        args.push_back(s);
      }

      if (args.size() != props + 1 || args.back() != "0") {
        TINY_SAT_LOG_ERROR("File Reader", "clause line error!");
      }

      Clause clause;
      for (size_t i = 0; i < args.size() - 1; ++i) {
        auto &arg = args[i];
        int prop = std::stoi(arg);
        bool positive = prop < 0 ? false : true;
        clause.add(Literal(Proposition(abs(prop)), positive));
      }
      cnf.add(clause);
    }
  }
}

}  // namespace tiny_sat
