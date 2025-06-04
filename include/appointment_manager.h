#pragma once

#include "csv.h"
#include "offer_manager.h"
#include <chrono>
#include <string>
#include <vector>

class Appointment {
public:
  std::string client_name;
  std::string offer_name;
  std::chrono::year_month_day date;
  std::chrono::minutes time;

  Appointment(std::string client_name, std::string offer_name,
              std::chrono::year_month_day date, std::chrono::minutes time);

  static std::chrono::year_month_day parseDate(const std::string &date_str);
  static std::chrono::minutes parseTime(const std::string &time_str);

  static Appointment deserialize(const std::vector<std::string> &values);
  static std::vector<std::string> serialize(const Appointment &entry);
};

class AppointmentSlot {
public:
  std::chrono::minutes start_time;
  std::chrono::minutes end_time;

  AppointmentSlot(std::chrono::minutes start_time,
                  std::chrono::minutes end_time);
};

class AppointmentManager : public CsvFile<Appointment> {
public:
  AppointmentManager();

  std::vector<AppointmentSlot>
  getAvailableSlots(const std::chrono::year_month_day &date,
                    const OfferManager &offer_manager) const;
  bool canScheduleOffer(const std::chrono::year_month_day &date,
                        const std::chrono::minutes &time, const Offer &offer,
                        const OfferManager &offer_manager) const;
};
