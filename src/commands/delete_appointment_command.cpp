#include "commands/delete_appointment_command.h"
#include "appointment_manager.h"
#include "ansi.h" // Added for colors
#include <iostream>
#include <iomanip> // For std::setw and std::setfill

DeleteAppointmentCommand::DeleteAppointmentCommand()
    : Command({"delete_appointment"}) {}

int DeleteAppointmentCommand::execute(std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cerr << COLOR("Usage: delete_appointment <client_name> <date (dd/mm/yyyy)> <time (hh:mm)>", BRED)
              << std::endl;
    return 1;
  }

  auto client_name = args[0];
  std::chrono::year_month_day date;
  std::chrono::minutes time;

  try {
    date = Appointment::parseDate(args[1]);
    time = Appointment::parseTime(args[2]);
  } catch (const std::exception& e) {
    std::cerr << COLOR("Error: Invalid date or time format.", BRED) << std::endl;
    std::cerr << COLOR("Please use dd/mm/yyyy for date and hh:mm for time.", BRED) << std::endl;
    return 1;
  }
  
  std::chrono::hh_mm_ss<std::chrono::minutes> time_display(time);
  // Reformat date for display
  std::ostringstream date_stream;
  date_stream << std::setfill('0') << std::setw(2) << (unsigned)date.day() << "/"
              << std::setfill('0') << std::setw(2) << (unsigned)date.month() << "/"
              << date.year();
  std::string formatted_date = date_stream.str();

  std::ostringstream time_stream;
  time_stream << std::setfill('0') << std::setw(2) << time_display.hours().count() << ":"
              << std::setfill('0') << std::setw(2) << time_display.minutes().count();
  std::string formatted_time = time_stream.str();

  AppointmentManager appointment_manager;
  auto removed_entry = appointment_manager.removeEntry(
      [&client_name, &date, &time](const Appointment &appointment) { // Pass by reference
        return appointment.client_name == client_name &&
               appointment.date == date && appointment.time == time;
      });

  if (removed_entry) {
    std::cout << COLOR("Appointment deleted: ", BGRN) << client_name << COLOR(", ", BGRN) << formatted_date << COLOR(", ", BGRN)
              << formatted_time << std::endl;
  } else {
    std::cerr << COLOR("No appointment found for: ", BRED) << client_name << COLOR(", ", BRED) << formatted_date
              << COLOR(", ", BRED) << formatted_time << std::endl;
  }

  return 0;
}

