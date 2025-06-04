#include "appointment_manager.h"
#include <algorithm>
#include <sstream>
#include <utility>

Appointment::Appointment(std::string client_name, std::string offer_name,
                         std::chrono::year_month_day date,
                         std::chrono::minutes time)
    : client_name{client_name}, offer_name{offer_name}, date{date}, time{time} {
}

std::chrono::year_month_day
Appointment::parseDate(const std::string &date_str) {
  std::istringstream is(date_str);
  std::chrono::year_month_day date;

  is >> std::chrono::parse("%d/%m/%Y", date);

  return date;
}

std::chrono::minutes Appointment::parseTime(const std::string &time_str) {
  std::istringstream is(time_str);
  std::chrono::minutes duration;

  is >> std::chrono::parse("%H:%M", duration);

  return duration;
}

std::vector<std::string> Appointment::serialize(const Appointment &entry) {
  std::chrono::hh_mm_ss<std::chrono::minutes> time(entry.time);

  std::vector<std::string> values;
  values.push_back(entry.client_name);
  values.push_back(entry.offer_name);
  values.push_back(std::format("{:2}/{:2}/{:4}", entry.date.day(),
                               entry.date.month(), entry.date.year()));
  values.push_back(std::format("{:2}:{:2}", time.hours(), time.minutes()));

  return values;
}

Appointment Appointment::deserialize(const std::vector<std::string> &values) {
  if (values.size() != 4) {
    throw std::invalid_argument("Expected 4 values for Appointment");
  }

  return Appointment(values[0], values[1], parseDate(values[2]),
                     parseTime(values[3]));
}

AppointmentSlot::AppointmentSlot(std::chrono::minutes start_time,
                                 std::chrono::minutes end_time)
    : start_time{start_time}, end_time{end_time} {}

AppointmentManager::AppointmentManager() : CsvFile("./data/appointments.csv") {}

std::vector<AppointmentSlot>
AppointmentManager::getAvailableSlots(const std::chrono::year_month_day &date,
                                      const OfferManager &offer_manager) const {

  std::vector<std::pair<std::chrono::minutes, std::chrono::minutes>>
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

      booked_slots.emplace_back(
          make_pair(appointment.time, appointment.time + offer->duration));
    }
  }

  std::sort(booked_slots.begin(), booked_slots.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  std::chrono::minutes start(9 * 60);
  std::chrono::minutes end(17 * 60);

  if (booked_slots.empty()) {
    return {AppointmentSlot(start, end)};
  }

  std::vector<std::pair<std::chrono::minutes, std::chrono::minutes>>
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
      available_slots.emplace_back(AppointmentSlot(start, slot.first));
    }
    start = slot.second;
  }

  if (start < end) {
    available_slots.emplace_back(AppointmentSlot(start, end));
  }

  return available_slots;
}
