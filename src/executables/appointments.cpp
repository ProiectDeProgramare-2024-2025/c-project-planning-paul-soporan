#include "commands/check_availability_command.h"
#include "commands/delete_appointment_command.h"
#include "commands/schedule_appointment_command.h"
#include "commands/view_appointments_command.h"
#include "executable.h"
#include "offer_manager.h"
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
  auto view_appointments_command = ViewAppointmentsCommand();
  auto delete_appointment_command = DeleteAppointmentCommand();
  auto check_availability_command = CheckAvailabilityCommand();
  auto schedule_appointment_command = ScheduleAppointmentCommand();

  Executable executable(std::vector<Command *>{
      {&view_appointments_command, &delete_appointment_command,
       &check_availability_command, &schedule_appointment_command}});

  return executable.run(argc, argv);
}
