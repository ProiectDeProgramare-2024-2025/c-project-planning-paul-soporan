#include "commands/add_offer_command.h"
#include "offer_manager.h"
#include <iostream>

AddOfferCommand::AddOfferCommand() : Command({"add_offer"}) {}

int AddOfferCommand::execute(std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cerr << "Usage: add_offer <name> <price> <duration>" << std::endl;
    return 1;
  }

  std::string name = args[0];
  int price = std::stoi(args[1]);
  int duration = std::stoi(args[2]);

  OfferManager offer_manager;
  offer_manager.addEntry(Offer(name, price, duration));

  std::cout << "Offer added: " << name << ", Price: " << price
            << ", Duration: " << duration << std::endl;
  return 0;
}
