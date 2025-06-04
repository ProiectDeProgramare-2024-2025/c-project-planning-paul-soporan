#include "commands/delete_appointment_command.h"
#include "appointment_manager.h"
#include <iostream>

DeleteAppointmentCommand::DeleteAppointmentCommand()
    : Command({"delete_appointment"}) {}

int DeleteAppointmentCommand::execute(std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cerr << "Usage: delete_appointment <client_name> <date> <time>"
              << std::endl;

    return 1;
  }

  auto client_name = args[0];
  auto date = Appointment::parseDate(args[1]);
  auto time = Appointment::parseTime(args[2]);

  AppointmentManager appointment_manager;
  auto removed_entry = appointment_manager.removeEntry(
      [client_name, date, time](const Appointment &appointment) {
        return appointment.client_name == client_name &&
               appointment.date == date && appointment.time == time;
      });

  if (removed_entry) {
    std::cout << "Appointment deleted: " << client_name << ", " << date << ", "
              << time << std::endl;
  } else {
    std::cerr << "No appointment found for: " << client_name << ", " << date
              << ", " << time << std::endl;
  }

  return 0;
}
