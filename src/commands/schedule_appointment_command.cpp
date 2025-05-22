#include "commands/schedule_appointment_command.h"
#include "appointment_manager.h"
#include <iostream>

ScheduleAppointmentCommand::ScheduleAppointmentCommand()
    : Command({"schedule_appointment"}) {}

int ScheduleAppointmentCommand::execute(std::vector<std::string> args) {
  if (args.size() < 4) {
    std::cerr << "Usage: schedule_appointment <client_name> <offer_name> "
                 "<date> <time>"
              << std::endl;

    return 1;
  }

  auto client_name = args[0];
  auto offer_name = args[1];
  auto date = Appointment::parseDate(args[2]);
  auto time = Appointment::parseTime(args[3]);

  return 0;
}
