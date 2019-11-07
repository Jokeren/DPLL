#include "dimacs.h"

#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <random>

#include <cstdlib>

#include "common.h"


namespace tiny_sat {

bool DIMACS::open(const std::string &file_path) {
  file_.open(file_path, std::ios::in | std::ios::out | std::ios::app);

  if (!file_.good()) {
    TINY_SAT_LOG_WARNING("File Reader", "open file error!");
    return false;
  }
  return true;
}


void DIMACS::read(CNF &cnf) {
  std::string buf;
  int props = 0;
  int clauses = 0;

  while (std::getline(file_, buf)) {
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
      
      while (ss >> s) {
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
      
      while (ss >> s) {
        args.push_back(s);
      }

      if (args.back() != "0") {
        TINY_SAT_LOG_ERROR("File Reader", "clause line error!");
      }

      Clause clause;
      for (size_t i = 0; i < args.size() - 1; ++i) {
        auto &arg = args[i];
        int prop = std::stoi(arg);
        bool positive = prop < 0 ? false : true;
        clause.add(Literal(abs(prop), positive));
      }
      cnf.add(clause);
    }
  }
}


void DIMACS::generate(int propositions, int clauses, int literals) {
  std::mt19937 generator;
  std::uniform_int_distribution<int> distribution(-propositions, propositions);
  std::set<std::set<int> > dict;
  std::set<int> clause;
  std::stringstream ss;

  ss << "p cnf " << propositions << " " << clauses << std::endl;
  for (size_t i = 0; i < clauses; ++i) {
    do {
      clause.clear();
      while (clause.size() != literals) {
        int num = distribution(generator);
        if (num == 0) {
          continue;
        }
        if (clause.find(num) == clause.end() && clause.find(-num) == clause.end()) {
          clause.insert(num);
        }
      }
    } while (dict.find(clause) != dict.end());

    for (auto l : clause) {
      ss << l << " ";
    }
    ss << "0" << std::endl;
  }

  file_ << ss.str() << std::endl;
}

}  // namespace tiny_sat
