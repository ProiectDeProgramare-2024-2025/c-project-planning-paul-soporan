#include "executable.h"
#include "ansi.h" // Added for colors
#include <cstring> // For strcmp
#include <iostream>
#include <vector> // Added for std::vector
#include <string> // Added for std::string

Executable::Executable(std::vector<Command *> commands) : commands{commands} {}

int Executable::run(int argc, char *argv[]) {
  if (argc < 2) { // Check if at least one command is provided
    std::cerr << COLOR("Error: No command provided.", BRED) << std::endl;
    // You might want to print available commands here
    std::cerr << COLOR("Usage: ./app <command> [args...]", YEL) << std::endl;
    return 1;
  }
  char *command_name = argv[1]; // Renamed for clarity

  for (auto cmd : commands) {
    for (auto path : cmd->paths) {
      if (strcmp(path.c_str(), command_name) == 0) {
        // Prepare arguments for the command's execute method
        std::vector<std::string> cmd_args;
        if (argc > 2) {
          for (int i = 2; i < argc; ++i) {
            cmd_args.push_back(argv[i]);
          }
        }
        return cmd->execute(cmd_args);
      }
    }
  }

  std::cerr << COLOR("Unknown command: ", BRED) << command_name << std::endl;
  // You could list available commands here as well for better UX
  std::cerr << COLOR("Type './app help' or consult documentation for available commands.", YEL) << std::endl;


  return 1;
}
