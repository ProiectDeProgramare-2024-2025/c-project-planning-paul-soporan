#include "commands/view_appointments_command.h"
#include "appointment_manager.h"
#include <iostream>

ViewAppointmentsCommand::ViewAppointmentsCommand()
    : Command({"view_appointments"}) {}

int ViewAppointmentsCommand::execute(std::vector<std::string>) {
  AppointmentManager appointment_manager;
  const auto &appointments = appointment_manager.getEntries();

  std::cout << "Appointments:" << std::endl;
  for (const auto &appointment : appointments) {
    std::cout << "Client: " << appointment.client_name
              << ", Offer: " << appointment.offer_name
              << ", Date: " << appointment.date << ", Time: "
              << std::chrono::hh_mm_ss<std::chrono::minutes>(appointment.time)
              << std::endl;
  }

  return 0;
}
