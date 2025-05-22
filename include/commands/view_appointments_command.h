#pragma once

#include "command.h"

class ViewAppointmentsCommand : public Command {
public:
  ViewAppointmentsCommand();

  int execute(std::vector<std::string> args) override;
};
