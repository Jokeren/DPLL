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
    ("o,output", "Output path of a DIMACS file", cxxopts::value<std::string>())
    ("p,propositions", "Number of propositions in a formula", cxxopts::value<int>())
    ("c,clauses", "Number of clauses in a formula", cxxopts::value<int>())
    ("l,literals", "Number of literals of per clause", cxxopts::value<int>())
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
  std::string output = result["o"].count() ? result["o"].as<std::string>() :
    (result["output"].count() ?  result["output"].as<std::string>() : "");

  if (input.size() == 0 && output.size() == 0) {
    TINY_SAT_LOG_INFO("Help", options.help().c_str());
    return 0;
  } else if (input.size() != 0 && output.size() != 0) {
    TINY_SAT_LOG_INFO("Help", options.help().c_str());
    return 0;
  } else if (input.size() != 0) {
    // Golver mode
    std::string solver_name = result["s"].count() ? result["s"].as<std::string>() : result["solver"].as<std::string>();
    tiny_sat::DIMACS dimacs;
    if (dimacs.open(input)) {
      tiny_sat::CNF cnf;
      dimacs.read(cnf);
#ifdef DEBUG
      TINY_SAT_LOG_INFO("Debug cnf", (cnf.to_string()).c_str());
#endif
      tiny_sat::Solver *solver = get_solver(solver_name);
      tiny_sat::Assignment assign(cnf.size());

      double elapsed_time = 0.0;
      struct timeval t1, t2;
      TINY_SAT_TIMER_START(elapsed_time, t1);

      if (solver->solve(cnf, assign)) {
        TINY_SAT_LOG_INFO("Result", (assign.to_string()).c_str());
      } else {
#ifdef DEBUG
        TINY_SAT_LOG_INFO("Debug assignment", (assign.to_string()).c_str());
#endif
        TINY_SAT_LOG_INFO("Result", "UNSAT");
      }
      TINY_SAT_LOG_INFO("Calls", (std::to_string(solver->calls())).c_str());

      TINY_SAT_TIMER_END(elapsed_time, t1, t2);
      TINY_SAT_LOG_INFO("Time", (std::to_string(elapsed_time)).c_str());
    }  
  } else {
    int propositions = result["p"].count() ? result["p"].as<int>() : result["propositions"].as<int>();
    int clauses = result["c"].count() ? result["c"].as<int>() : result["clauses"].as<int>();
    int literals = result["l"].count() ? result["l"].as<int>() : result["literals"].as<int>();

    // Generator mode
    tiny_sat::DIMACS dimacs;
    if (dimacs.open(output)) {
      dimacs.generate(propositions, clauses, literals);
    }  
  }

  return 0;
}
