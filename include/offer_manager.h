#pragma once

#include "csv.h"
#include <chrono>
#include <string>
#include <vector>

class Offer {
public:
  std::string name;
  int price;
  std::chrono::minutes duration;

  Offer(std::string name, int price, int duration);

  static Offer deserialize(const std::vector<std::string> &values);
  static std::vector<std::string> serialize(const Offer &entry);
};

class OfferManager : public CsvFile<Offer> {
public:
  OfferManager();
};
