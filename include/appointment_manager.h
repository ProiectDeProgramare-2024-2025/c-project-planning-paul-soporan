#pragma once

#include "csv.h"
#include <chrono>
#include <string>
#include <vector>

class Appointment {
public:
  std::string client_name;
  std::string offer_name;
  std::chrono::year_month_day date;
  std::chrono::hh_mm_ss<std::chrono::minutes> time;

  Appointment(std::string client_name, std::string offer_name,
              std::chrono::year_month_day date,
              std::chrono::hh_mm_ss<std::chrono::minutes> time);
};

class AppointmentBuilder : private CsvEntryBuilder<Appointment> {
public:
  Appointment deserialize(const std::vector<std::string> &values) const;
  std::vector<std::string> serialize(const Appointment &entry) const;
};

class AppointmentManager : public CsvFile<Appointment, AppointmentBuilder> {
public:
  AppointmentManager();
};
