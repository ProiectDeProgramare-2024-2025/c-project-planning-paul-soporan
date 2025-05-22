#include "offer_manager.h"
#include <fstream>

Offer::Offer(std::string name, int price, int duration)
    : name{name}, price{price}, duration{duration} {}

std::vector<std::string> OfferBuilder::serialize(const Offer &entry) const {
  std::vector<std::string> values;
  values.push_back(entry.name);
  values.push_back(std::to_string(entry.price));
  values.push_back(std::to_string(entry.duration));

  return values;
}

Offer OfferBuilder::deserialize(const std::vector<std::string> &values) const {
  if (values.size() != 3) {
    throw std::invalid_argument("Expected 3 values for Offer");
  }

  return Offer(values[0], std::stoi(values[1]), std::stoi(values[2]));
}

OfferManager::OfferManager() : CsvFile("./data/offers.csv") {}
