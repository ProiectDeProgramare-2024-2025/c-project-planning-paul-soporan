#include "command.h"

class DeleteOfferCommand : public Command {
public:
  DeleteOfferCommand();

  int execute(std::vector<std::string> args) override;
};
