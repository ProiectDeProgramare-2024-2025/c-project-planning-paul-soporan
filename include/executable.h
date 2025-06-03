#pragma once

#include <command.h>
#include <vector>

class Executable {
private:
  std::vector<Command *> commands;

public:
  Executable(std::vector<Command *> commands);

  int run(int argc, char *argv[]);
};
