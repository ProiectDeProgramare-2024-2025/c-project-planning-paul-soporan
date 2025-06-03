#include "executable.h"
#include "cstring"
#include "iostream"

Executable::Executable(std::vector<Command *> commands) : commands{commands} {}

int Executable::run(int argc, char *argv[]) {
  char *command = argv[1];

  for (auto cmd : commands) {
    for (auto path : cmd->paths) {
      if (strcmp(path.c_str(), command) == 0) {
        return cmd->execute(std::vector<std::string>(argv + 2, argv + argc));
      }
    }
  }

  std::cerr << "Unknown command: " << command << std::endl;

  return 1;
}
