#include "command.h"

class DeleteAppointmentCommand : public Command {
public:
  DeleteAppointmentCommand();

  int execute(std::vector<std::string> args) override;
};
