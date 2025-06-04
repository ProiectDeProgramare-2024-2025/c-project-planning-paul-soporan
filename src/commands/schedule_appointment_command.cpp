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

  OfferManager offer_manager;
  AppointmentManager appointment_manager;

  auto offer_entries = offer_manager.getEntries();
  auto offer = std::find_if(
      offer_entries.begin(), offer_entries.end(),
      [&offer_name](const Offer &offer) { return offer.name == offer_name; });

  if (offer == offer_entries.end()) {
    std::cerr << "No offer found with name: " << offer_name << std::endl;
    return 1;
  }

  if (!appointment_manager.canScheduleOffer(date, time, *offer,
                                            offer_manager)) {
    std::cerr << "Cannot schedule an " << offer_name << " appointment for "
              << client_name << " at " << date << " " << time
              << ". The slot is not available." << std::endl;
    return 1;
  }

  appointment_manager.addEntry(
      Appointment(client_name, offer->name, date, time));

  return 0;
}
