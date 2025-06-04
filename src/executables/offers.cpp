#include "commands/add_offer_command.h"
#include "commands/delete_offer_command.h"
#include "commands/view_offers_command.h"
#include "executable.h"

int main(int argc, char *argv[]) {
  auto add_offer_command = AddOfferCommand();
  auto delete_offer_command = DeleteOfferCommand();
  auto view_offers_command = ViewOffersCommand();

  Executable executable(std::vector<Command *>{{
      &add_offer_command,
      &delete_offer_command,
      &view_offers_command,
  }});

  return executable.run(argc, argv);
}
