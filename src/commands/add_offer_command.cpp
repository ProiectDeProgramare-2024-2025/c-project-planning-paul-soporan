#include "commands/add_offer_command.h"
#include "offer_manager.h"
#include "ansi.h" // Added for colors
#include <iostream>

AddOfferCommand::AddOfferCommand() : Command({"add_offer"}) {}

int AddOfferCommand::execute(std::vector<std::string> args) {
  if (args.size() < 3) {
    std::cerr << COLOR("Usage: add_offer <name> <price> <duration>", BRED) << std::endl;
    return 1;
  }

  std::string name = args[0];
  int price = 0;
  int duration = 0;

  try {
    price = std::stoi(args[1]);
    duration = std::stoi(args[2]);
  } catch (const std::invalid_argument& ia) {
    std::cerr << COLOR("Error: Invalid number format for price or duration.", BRED) << std::endl;
    return 1;
  } catch (const std::out_of_range& oor) {
    std::cerr << COLOR("Error: Price or duration out of range.", BRED) << std::endl;
    return 1;
  }


  OfferManager offer_manager;
  offer_manager.addEntry(Offer(name, price, duration));

  std::cout << COLOR("Offer added: ", BGRN) << name << COLOR(", Price: ", BGRN) << price
            << COLOR(", Duration: ", BGRN) << duration << std::endl;
  return 0;
}
