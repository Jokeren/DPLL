#include <cxxopts.hpp>

Solver *get_solver(const std::string &name) {
  if (name == "tiny") {
    return new TinySolver();
  }
}

int main(int argc, char *argv[]) {
  cxxopts::Options options("tinySAT", "minimal SAT solver implementation");
  options.add_options()
    ("i,input", "Input path of a DIMACS file", cxxopts::value<std::string>())
    ("s,solver", "Choose a SAT solver (default tiny):\nRandom solver (random)\nTwo clause solver (two)\nTiny solver (tine)n",
     cxxopts::value(std::string>("tiny")))
    ("h,help", "tinySAT help");

  if (argc == 1) {
    return 0;
  }

  auto result = options.parse(argc, argv);

  if (result["help"].count() || result["h"].count()) {
    return 0;
  }

  std::string input = result["i"].count() ? result["i"].as<std::string>() : result["input"].as<std::string>();
  std::string solver_name = result["s"].count() ? result["s"].as<std::string>() : result["solver"].as<std::string>();

  DIMACS dimacs;
  if (dimacs.open(input)) {
    CNF cnf;
    dimacs.read(cnf);

    Solver *solver = get_solver(solver_name);
    if (solver->solve(cnf)) {
    } else {
    }
  }  

  return 0;
}
