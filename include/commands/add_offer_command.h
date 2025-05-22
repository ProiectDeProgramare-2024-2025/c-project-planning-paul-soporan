#include "command.h"

class AddOfferCommand : public Command {
public:
  AddOfferCommand();

  int execute(std::vector<std::string> args) override;
};
