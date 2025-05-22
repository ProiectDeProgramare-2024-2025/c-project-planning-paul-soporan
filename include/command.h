#pragma once

#include <string>
#include <vector>

class Command {
public:
  std::vector<std::string> paths;

  Command(std::vector<std::string> paths);

  virtual int execute(std::vector<std::string> args) = 0;
};
