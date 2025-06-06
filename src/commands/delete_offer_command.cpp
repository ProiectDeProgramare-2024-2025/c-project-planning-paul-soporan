#include "commands/delete_offer_command.h"
#include "offer_manager.h"
#include "ansi.h" // Added for colors
#include <iostream>

DeleteOfferCommand::DeleteOfferCommand() : Command({"delete_offer"}) {}

int DeleteOfferCommand::execute(std::vector<std::string> args) {
  if (args.size() < 1) {
    std::cerr << COLOR("Usage: delete_offer <name>", BRED) << std::endl;
    return 1;
  }

  std::string name = args[0];

  OfferManager offer_manager;
  auto removed_entry = offer_manager.removeEntry(
      [&name](const Offer &offer) { return offer.name == name; }); // Pass by reference

  if (removed_entry) {
    std::cout << COLOR("Offer deleted: ", BGRN) << name << std::endl;
  } else {
    std::cerr << COLOR("No offer found with name: ", BRED) << name << std::endl;
  }

  return 0;
}
