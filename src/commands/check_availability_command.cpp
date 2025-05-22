#include "commands/check_availability_command.h"
#include "appointment_manager.h"
#include <iostream>

CheckAvailabilityCommand::CheckAvailabilityCommand()
    : Command({"check_availability"}) {}

int CheckAvailabilityCommand::execute(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << "Usage: check_availability <date>" << std::endl;

    return 1;
  }

  auto date = Appointment::parseDate(args[0]);

  return 0;
}
