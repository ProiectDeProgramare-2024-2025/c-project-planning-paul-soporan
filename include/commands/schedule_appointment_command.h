#pragma once

#include "command.h"

class ScheduleAppointmentCommand : public Command {
public:
  ScheduleAppointmentCommand();

  int execute(std::vector<std::string> args) override;
};
