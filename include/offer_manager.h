#pragma once

#include "csv.h"
#include <string>
#include <vector>

class Offer {
public:
  std::string name;
  int price;
  int duration;

  Offer(std::string name, int price, int duration);
};

class OfferBuilder : private CsvEntryBuilder<Offer> {
public:
  Offer deserialize(const std::vector<std::string> &values) const;
  std::vector<std::string> serialize(const Offer &entry) const;
};

class OfferManager : public CsvFile<Offer, OfferBuilder> {
public:
  OfferManager();
};
