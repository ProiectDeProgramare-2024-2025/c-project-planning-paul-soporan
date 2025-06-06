#include "commands/schedule_appointment_command.h"
#include "appointment_manager.h"
#include "ansi.h" // Added for colors
#include <iostream>
#include <iomanip> // For std::setw and std::setfill

ScheduleAppointmentCommand::ScheduleAppointmentCommand()
    : Command({"schedule_appointment"}) {}

int ScheduleAppointmentCommand::execute(std::vector<std::string> args) {
  if (args.size() < 4) {
    std::cerr << COLOR("Usage: schedule_appointment <client_name> <offer_name> <date (dd/mm/yyyy)> <time (hh:mm)>", BRED)
              << std::endl;
    return 1;
  }

  auto client_name = args[0];
  auto offer_name = args[1];
  std::chrono::year_month_day date;
  std::chrono::minutes time;

  try {
    date = Appointment::parseDate(args[2]);
    time = Appointment::parseTime(args[3]);
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

  std::ostringstream time_stream_display;
  time_stream_display << std::setfill('0') << std::setw(2) << time_display.hours().count() << ":"
              << std::setfill('0') << std::setw(2) << time_display.minutes().count();
  std::string formatted_time_display = time_stream_display.str();


  OfferManager offer_manager;
  AppointmentManager appointment_manager;

  auto offer_entries = offer_manager.getEntries();
  auto offer_it = std::find_if( // Renamed to offer_it to avoid conflict
      offer_entries.begin(), offer_entries.end(),
      [&offer_name](const Offer &o) { return o.name == offer_name; }); // Pass by reference, changed variable name

  if (offer_it == offer_entries.end()) {
    std::cerr << COLOR("No offer found with name: ", BRED) << offer_name << std::endl;
    return 1;
  }

  if (!appointment_manager.canScheduleOffer(date, time, *offer_it, // Use offer_it
                                            offer_manager)) {
    std::cerr << COLOR("Cannot schedule an '", BRED) << offer_name
              << COLOR("' appointment for ", BRED) << client_name
              << COLOR(" at ", BRED) << formatted_date << " " << formatted_time_display
              << COLOR(". The slot is not available or conflicts with working hours (09:00-17:00).", BRED) << std::endl;
    return 1;
  }

  appointment_manager.addEntry(
      Appointment(client_name, offer_it->name, date, time)); // Use offer_it

  std::cout << COLOR("Appointment scheduled successfully for ", BGRN) << client_name
            << COLOR(" for service '", BGRN) << offer_it->name // Use offer_it
            << COLOR("' on ", BGRN) << formatted_date
            << COLOR(" at ", BGRN) << formatted_time_display << std::endl;

  return 0;
}
