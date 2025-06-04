#include "commands/check_availability_command.h"
#include "appointment_manager.h"
#include "offer_manager.h"
#include <iostream>

CheckAvailabilityCommand::CheckAvailabilityCommand()
    : Command({"check_availability"}) {}

int CheckAvailabilityCommand::execute(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << "Usage: check_availability <date>" << std::endl;

    return 1;
  }

  auto date = Appointment::parseDate(args[0]);

  OfferManager offer_manager;
  AppointmentManager appointment_manager;

  auto slots = appointment_manager.getAvailableSlots(date, offer_manager);
  if (slots.empty()) {
    std::cout << "No available slots for " << date << std::endl;
  } else {
    std::cout << "Available slots for " << date << ":" << std::endl;
    for (const auto &slot : slots) {
      std::cout << "From "
                << std::chrono::hh_mm_ss<std::chrono::minutes>(slot.start_time)
                << " to "
                << std::chrono::hh_mm_ss<std::chrono::minutes>(slot.end_time)
                << std::endl;
    }
  }

  return 0;
}
