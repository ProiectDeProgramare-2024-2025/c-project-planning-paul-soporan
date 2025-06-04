#include "appointment_manager.h"
#include <algorithm>
#include <sstream>
#include <utility>

Appointment::Appointment(std::string client_name, std::string offer_name,
                         std::chrono::year_month_day date,
                         std::chrono::hh_mm_ss<std::chrono::minutes> time)
    : client_name{client_name}, offer_name{offer_name}, date{date}, time{time} {
}

std::chrono::year_month_day
Appointment::parseDate(const std::string &date_str) {
  std::istringstream is(date_str);
  std::chrono::year_month_day date;

  is >> std::chrono::parse("%d/%m/%Y", date);

  return date;
}

std::chrono::hh_mm_ss<std::chrono::minutes>
Appointment::parseTime(const std::string &time_str) {
  std::istringstream is(time_str);
  std::chrono::minutes minutes;

  is >> std::chrono::parse("%H:%M", minutes);

  return std::chrono::hh_mm_ss<std::chrono::minutes>(minutes);
}

std::vector<std::string> Appointment::serialize(const Appointment &entry) {
  std::vector<std::string> values;
  values.push_back(entry.client_name);
  values.push_back(entry.offer_name);
  values.push_back(std::format("{:2}/{:2}/{:4}", entry.date.day(),
                               entry.date.month(), entry.date.year()));
  values.push_back(
      std::format("{:2}:{:2}", entry.time.hours(), entry.time.minutes()));

  return values;
}

Appointment Appointment::deserialize(const std::vector<std::string> &values) {
  if (values.size() != 4) {
    throw std::invalid_argument("Expected 4 values for Appointment");
  }

  return Appointment(values[0], values[1], parseDate(values[2]),
                     parseTime(values[3]));
}

AppointmentSlot::AppointmentSlot(
    std::chrono::hh_mm_ss<std::chrono::minutes> start_time,
    std::chrono::hh_mm_ss<std::chrono::minutes> end_time)
    : start_time{start_time}, end_time{end_time} {}

AppointmentManager::AppointmentManager() : CsvFile("./data/appointments.csv") {}

std::vector<AppointmentSlot>
AppointmentManager::getAvailableSlots(const std::chrono::year_month_day &date,
                                      const OfferManager &offer_manager) const {

  std::vector<std::pair<std::chrono::duration<int, std::ratio<60>>,
                        std::chrono::duration<int, std::ratio<60>>>>
      booked_slots;

  for (const auto &appointment : getEntries()) {
    if (appointment.date == date) {
      auto offer_entries = offer_manager.getEntries();

      auto offer = std::find_if(offer_entries.begin(), offer_entries.end(),
                                [&appointment](const Offer &offer) {
                                  return offer.name == appointment.offer_name;
                                });

      if (offer == offer_entries.end()) {
        throw std::runtime_error("No offer found for appointment: " +
                                 appointment.offer_name);
      }

      booked_slots.emplace_back(make_pair(
          appointment.time.hours(),
          appointment.time.hours() + appointment.time.minutes() +
              std::chrono::duration<int, std::ratio<60>>(offer->duration)));
    }
  }

  std::sort(booked_slots.begin(), booked_slots.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  std::chrono::duration<int, std::ratio<60>> start(9 * 60);
  std::chrono::duration<int, std::ratio<60>> end(17 * 60);

  if (booked_slots.empty()) {
    return {AppointmentSlot(std::chrono::hh_mm_ss<std::chrono::minutes>(start),
                            std::chrono::hh_mm_ss<std::chrono::minutes>(end))};
  }

  std::vector<std::pair<std::chrono::duration<int, std::ratio<60>>,
                        std::chrono::duration<int, std::ratio<60>>>>
      next_booked_slots;

  auto last = booked_slots.front();
  for (auto i = 1; i < booked_slots.size(); ++i) {
    auto current = booked_slots[i];
    if (current.first > last.second) {
      next_booked_slots.emplace_back(last.first, current.first);
      last = current;
    } else {
      last.second = std::max(last.second, current.second);
    }
  }

  next_booked_slots.emplace_back(last.first, last.second);

  std::vector<AppointmentSlot> available_slots;

  for (const auto &slot : next_booked_slots) {
    if (slot.first > start) {
      available_slots.emplace_back(AppointmentSlot(
          std::chrono::hh_mm_ss<std::chrono::minutes>(start),
          std::chrono::hh_mm_ss<std::chrono::minutes>(slot.first)));
    }
    start = slot.second;
  }

  if (start < end) {
    available_slots.emplace_back(
        AppointmentSlot(std::chrono::hh_mm_ss<std::chrono::minutes>(start),
                        std::chrono::hh_mm_ss<std::chrono::minutes>(end)));
  }

  return available_slots;
}
