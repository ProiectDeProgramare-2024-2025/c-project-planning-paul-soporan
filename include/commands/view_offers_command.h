#include "command.h"

class ViewOffersCommand : public Command {
public:
  ViewOffersCommand();

  int execute(std::vector<std::string> args) override;
};
