#include "commands/check_availability_command.h"
#include "appointment_manager.h"
#include "offer_manager.h"
#include "ansi.h" // Added for colors
#include <iostream>
#include <iomanip> // For std::setw and std::setfill

CheckAvailabilityCommand::CheckAvailabilityCommand()
    : Command({"check_availability"}) {}

int CheckAvailabilityCommand::execute(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << COLOR("Usage: check_availability <date (dd/mm/yyyy)>", BRED) << std::endl;
    return 1;
  }

  std::chrono::year_month_day date;
  try {
    date = Appointment::parseDate(args[0]);
  } catch (const std::exception& e) {
    std::cerr << COLOR("Error: Invalid date format. Please use dd/mm/yyyy.", BRED) << std::endl;
    return 1;
  }

  OfferManager offer_manager;
  AppointmentManager appointment_manager;

  auto slots = appointment_manager.getAvailableSlots(date, offer_manager);
  
  // Reformat date for display
  std::ostringstream date_stream;
  date_stream << std::setfill('0') << std::setw(2) << (unsigned)date.day() << "/"
              << std::setfill('0') << std::setw(2) << (unsigned)date.month() << "/"
              << date.year();
  std::string formatted_date = date_stream.str();

  if (slots.empty()) {
    std::cout << COLOR("No available slots for ", YEL) << formatted_date << std::endl;
  } else {
    std::cout << COLOR("Available slots for ", BCYN) << formatted_date << ":" << std::endl;
    for (const auto &slot : slots) {
      std::chrono::hh_mm_ss<std::chrono::minutes> start_display(slot.start_time);
      std::chrono::hh_mm_ss<std::chrono::minutes> end_display(slot.end_time);
      std::cout << COLOR("  From ", CYN)
                << std::setfill('0') << std::setw(2) << start_display.hours().count() << ":"
                << std::setfill('0') << std::setw(2) << start_display.minutes().count()
                << COLOR(" to ", CYN)
                << std::setfill('0') << std::setw(2) << end_display.hours().count() << ":"
                << std::setfill('0') << std::setw(2) << end_display.minutes().count()
                << std::endl;
    }
  }

  return 0;
}

