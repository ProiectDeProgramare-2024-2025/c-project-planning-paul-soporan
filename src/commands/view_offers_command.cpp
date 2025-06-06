#include "commands/view_offers_command.h"
#include "offer_manager.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm> // For std::sort
#include <numeric>   // For std::accumulate (if calculating dynamic width)

// Helper function to print a row, handling potential truncation
void printOfferRow(const std::string& name, int price, int duration,
                   int nameWidth, int priceWidth, int durationWidth) {
    std::string displayName = name;
    if (displayName.length() > static_cast<size_t>(nameWidth)) {
        displayName = displayName.substr(0, nameWidth - 3) + "...";
    }

    std::cout << "| " << WHT << std::left << std::setw(nameWidth) << displayName << CRESET << " "
              << "| " << WHT << std::right << std::setw(priceWidth) << price << CRESET << " "
              << "| " << WHT << std::right << std::setw(durationWidth) << duration << CRESET << " |"
              << std::endl;
}

ViewOffersCommand::ViewOffersCommand() : Command({"view_offers"}) {}

int ViewOffersCommand::execute(std::vector<std::string> args) {
  OfferManager offer_manager;
  auto offers = offer_manager.getEntries(); // Make a mutable copy for sorting

  if (offers.empty()) {
    std::cout << COLOR("No offers available at the moment.", YEL) << std::endl;
    return 0;
  }

  // Sort offers by name
  std::sort(offers.begin(), offers.end(), [](const Offer& a, const Offer& b) {
    return a.name < b.name;
  });

  // Define column widths (can be dynamic or fixed)
  // For this example, let's use slightly more generous fixed widths
  // or calculate them dynamically if preferred.
  // Let's try fixed widths that are a bit larger.
  int nameColWidth = 25;
  int priceColWidth = 10; // "Price (RON)"
  int durationColWidth = 15; // "Duration (min)"

  // Header
  std::cout << COLOR("Available Offers (Sorted by Name):", BCYN) << std::endl;
  // Top border
  std::cout << "+-" << std::string(nameColWidth, '-') << "-+-"
            << std::string(priceColWidth, '-') << "-+-"
            << std::string(durationColWidth, '-') << "-+" << std::endl;

  // Column Titles
  std::cout << "| " << BYEL << std::left << std::setw(nameColWidth) << "Name" << CRESET << " "
            << "| " << BYEL << std::right << std::setw(priceColWidth) << "Price(RON)" << CRESET << " "
            << "| " << BYEL << std::right << std::setw(durationColWidth) << "Duration(min)" << CRESET << " |"
            << std::endl;

  // Middle border
  std::cout << "+-" << std::string(nameColWidth, '-') << "-+-"
            << std::string(priceColWidth, '-') << "-+-"
            << std::string(durationColWidth, '-') << "-+" << std::endl;

  // Data Rows
  for (const auto &offer : offers) {
    printOfferRow(offer.name, offer.price, offer.duration.count(),
                  nameColWidth, priceColWidth, durationColWidth);
  }

  // Bottom border
  std::cout << "+-" << std::string(nameColWidth, '-') << "-+-"
            << std::string(priceColWidth, '-') << "-+-"
            << std::string(durationColWidth, '-') << "-+" << std::endl;

  return 0;
}
