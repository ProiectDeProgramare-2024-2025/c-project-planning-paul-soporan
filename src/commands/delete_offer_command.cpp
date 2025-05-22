#include "commands/delete_offer_command.h"
#include "offer_manager.h"
#include <iostream>

DeleteOfferCommand::DeleteOfferCommand() : Command({"delete_offer"}) {}

int DeleteOfferCommand::execute(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << "Usage: delete_offer <name>" << std::endl;
    return 1;
  }

  std::string name = args[0];

  OfferManager offer_manager;
  offer_manager.removeEntry(
      [name](const Offer &offer) { return offer.name == name; });

  std::cout << "Offer deleted: " << name << std::endl;

  return 0;
}
