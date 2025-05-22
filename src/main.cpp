#include "commands/add_offer_command.h"
#include "commands/check_availability_command.h"
#include "commands/delete_appointment_command.h"
#include "commands/delete_offer_command.h"
#include "commands/schedule_appointment_command.h"
#include "commands/view_appointments_command.h"
#include "commands/view_offers_command.h"
#include "offer_manager.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  char *command = argv[1];

  auto add_offer_command = AddOfferCommand();
  auto delete_offer_command = DeleteOfferCommand();
  auto view_offers_command = ViewOffersCommand();

  auto view_appointments_command = ViewAppointmentsCommand();
  auto delete_appointment_command = DeleteAppointmentCommand();
  auto check_availability_command = CheckAvailabilityCommand();
  auto schedule_appointment_command = ScheduleAppointmentCommand();

  std::vector<Command *> commands = {
      {&add_offer_command, &delete_offer_command, &view_offers_command,
       &view_appointments_command, &delete_appointment_command,
       &check_availability_command, &schedule_appointment_command}};

  for (auto cmd : commands) {
    for (auto path : cmd->paths) {
      if (strcmp(path.c_str(), command) == 0) {
        return cmd->execute(std::vector<std::string>(argv + 2, argv + argc));
      }
    }
  }

  std::cerr << "Unknown command: " << command << std::endl;

  return 1;
}
