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

  static std::chrono::year_month_day parseDate(const std::string &date_str);
  static std::chrono::hh_mm_ss<std::chrono::minutes>
  parseTime(const std::string &time_str);

  static Appointment deserialize(const std::vector<std::string> &values);
  static std::vector<std::string> serialize(const Appointment &entry);
};

class AppointmentManager : public CsvFile<Appointment> {
public:
  AppointmentManager();
};
