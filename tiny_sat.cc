#include <tiny_sat.h>

#include <string>

#include <cxxopts.hpp>


tiny_sat::Solver *get_solver(const std::string &name) {
  if (name == "tiny") {
    return new tiny_sat::TinySolver();
  } else if (name == "random") {
    return new tiny_sat::RandomSolver();
  } else if (name == "two") {
    return new tiny_sat::TwoClauseSolver();
  }
  // Report error
}


int main(int argc, char *argv[]) {
  cxxopts::Options options("tinySAT", "minimal SAT solver implementation");
  options.add_options()
    ("i,input", "Input path of a DIMACS file", cxxopts::value<std::string>())
    ("s,solver", "Choose a SAT solver: random, two, tiny",
     cxxopts::value<std::string>()->default_value("tiny"))
    ("h,help", "tinySAT help");

  if (argc == 1) {
    TINY_SAT_LOG_INFO("Help", options.help().c_str());
    return 0;
  }

  auto result = options.parse(argc, argv);

  if (result["help"].count() || result["h"].count()) {
    TINY_SAT_LOG_INFO("Help", options.help().c_str());
    return 0;
  }

  std::string input = result["i"].count() ? result["i"].as<std::string>() :
    (result["input"].count() ?  result["input"].as<std::string>() : "");
  if (input.size() == 0) {
    TINY_SAT_LOG_INFO("Help", options.help().c_str());
    return 0;
  }

  std::string solver_name = result["s"].count() ? result["s"].as<std::string>() : result["solver"].as<std::string>();
  tiny_sat::DIMACS dimacs;
  if (dimacs.open(input)) {
    tiny_sat::CNF cnf;
    dimacs.read(cnf);

#ifdef DEBUG
    TINY_SAT_LOG_INFO("Debug cnf", ("\n" + cnf.to_string()).c_str());
#endif

    tiny_sat::Solver *solver = get_solver(solver_name);
    tiny_sat::Assignment assign;
    if (solver->solve(cnf, assign)) {
      TINY_SAT_LOG_INFO("Result", assign.to_string().c_str());
    } else {
      TINY_SAT_LOG_INFO("Result", "UNSAT");
    }
  }  

  return 0;
}
