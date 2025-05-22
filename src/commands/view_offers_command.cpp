#include "commands/view_offers_command.h"
#include "offer_manager.h"
#include <iostream>

ViewOffersCommand::ViewOffersCommand() : Command({"view_offers"}) {}

int ViewOffersCommand::execute(std::vector<std::string>) {
  OfferManager offer_manager;
  const auto &offers = offer_manager.getEntries();

  std::cout << "Offers:" << std::endl;
  for (const auto &offer : offers) {
    std::cout << "Name: " << offer.name << ", Price: " << offer.price
              << ", Duration: " << offer.duration << std::endl;
  }

  return 0;
}
