#pragma once

#include "command.h"

class CheckAvailabilityCommand : public Command {
public:
  CheckAvailabilityCommand();

  int execute(std::vector<std::string> args) override;
};
