#include "appointment_manager.h"
#include <algorithm>
#include <iomanip>   // Required for std::get_time
#include <sstream>   // Already here
#include <stdexcept> // Required for std::runtime_error
#include <utility>

Appointment::Appointment(std::string client_name, std::string offer_name,
                         std::chrono::year_month_day date,
                         std::chrono::minutes time)
    : client_name{std::move(client_name)}, offer_name{std::move(offer_name)},
      date{date}, time{time} {}

std::chrono::year_month_day
Appointment::parseDate(const std::string &date_str) {
  std::tm t{};
  std::istringstream ss(date_str);
  // Use std::get_time to parse the date string
  ss >> std::get_time(&t, "%d/%m/%Y");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse date string: " + date_str);
  }
  // std::tm month is 0-11, day is 1-31, year is years since 1900
  return std::chrono::year_month_day{
      std::chrono::year{t.tm_year + 1900},
      std::chrono::month{static_cast<unsigned>(t.tm_mon + 1)},
      std::chrono::day{static_cast<unsigned>(t.tm_mday)}};
}

std::chrono::minutes Appointment::parseTime(const std::string &time_str) {
  std::tm t{};
  std::istringstream ss(time_str);
  // Use std::get_time to parse the time string
  ss >> std::get_time(&t, "%H:%M");
  if (ss.fail()) {
    throw std::runtime_error("Failed to parse time string: " + time_str);
  }
  // Convert hours and minutes from std::tm to std::chrono::minutes
  return std::chrono::hours{t.tm_hour} + std::chrono::minutes{t.tm_min};
}

std::vector<std::string> Appointment::serialize(const Appointment &entry) {
  std::chrono::hh_mm_ss<std::chrono::minutes> time_display(
      entry.time); // Renamed variable

  std::vector<std::string> values;
  values.push_back(entry.client_name);
  values.push_back(entry.offer_name);

  // Use std::format if available and working, otherwise manual formatting
  // For wider compatibility, let's stick to ostringstream for formatting
  // date/time to string here
  std::ostringstream date_ss;
  date_ss << std::setfill('0') << std::setw(2)
          << static_cast<unsigned>(entry.date.day()) << '/' << std::setfill('0')
          << std::setw(2) << static_cast<unsigned>(entry.date.month()) << '/'
          << static_cast<int>(entry.date.year());
  values.push_back(date_ss.str());

  std::ostringstream time_ss;
  time_ss << std::setfill('0') << std::setw(2) << time_display.hours().count()
          << ':' << std::setfill('0') << std::setw(2)
          << time_display.minutes().count();
  values.push_back(time_ss.str());

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

AppointmentManager::AppointmentManager()
    : CsvFile("./data/appointments.csv") {} //

std::vector<AppointmentSlot>
AppointmentManager::getAvailableSlots(const std::chrono::year_month_day &date,
                                      const OfferManager &offer_manager) const {

  std::vector<std::pair<std::chrono::minutes, std::chrono::minutes>>
      booked_slots;

  for (const auto &appointment : getEntries()) {
    if (appointment.date == date) {
      auto offer_entries = offer_manager.getEntries();

      auto offer_it = std::find_if(
          offer_entries.begin(), offer_entries.end(), // Renamed to offer_it
          [&appointment](const Offer &o) {            // Renamed to o
            return o.name == appointment.offer_name;
          });

      if (offer_it == offer_entries.end()) {
        throw std::runtime_error("No offer found for appointment: " +
                                 appointment.offer_name);
      }

      booked_slots.emplace_back(appointment.time,
                                appointment.time +
                                    offer_it->duration); // Use offer_it
    }
  }

  std::sort(booked_slots.begin(), booked_slots.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  std::chrono::minutes start_working_day(9 * 60); // 09:00
  std::chrono::minutes end_working_day(17 * 60);  // 17:00
  std::chrono::minutes current_time = start_working_day;

  std::vector<AppointmentSlot> available_slots;

  if (booked_slots.empty()) {
    if (start_working_day < end_working_day) { // Ensure working day is valid
      available_slots.emplace_back(start_working_day, end_working_day);
    }
    return available_slots;
  }

  // Merge overlapping/adjacent booked_slots first
  std::vector<std::pair<std::chrono::minutes, std::chrono::minutes>>
      merged_booked_slots;
  if (!booked_slots.empty()) {
    merged_booked_slots.push_back(booked_slots.front());
    for (size_t i = 1; i < booked_slots.size(); ++i) {
      if (booked_slots[i].first <
          merged_booked_slots.back().second) { // Overlap or adjacent
        merged_booked_slots.back().second =
            std::max(merged_booked_slots.back().second, booked_slots[i].second);
      } else {
        merged_booked_slots.push_back(booked_slots[i]);
      }
    }
  }

  for (const auto &slot : merged_booked_slots) {
    // Add available slot before the current booked slot
    if (slot.first > current_time) {
      available_slots.emplace_back(current_time, slot.first);
    }
    // Move current_time to the end of the current booked slot
    current_time = std::max(current_time, slot.second);
  }

  // Add any remaining time slot at the end of the day
  if (current_time < end_working_day) {
    available_slots.emplace_back(current_time, end_working_day);
  }

  // Filter out slots that are outside working hours (e.g. if all slots are
  // booked or day ends early) This step might be redundant if current_time
  // logic is perfect but good for safety.
  available_slots.erase(
      std::remove_if(available_slots.begin(), available_slots.end(),
                     [&](const AppointmentSlot &slot) {
                       return slot.start_time >=
                                  slot.end_time || // Invalid slot
                              slot.end_time <=
                                  start_working_day || // Slot ends before start
                              slot.start_time >=
                                  end_working_day; // Slot starts after end
                     }),
      available_slots.end());

  return available_slots;
}

bool AppointmentManager::canScheduleOffer(
    const std::chrono::year_month_day &date, const std::chrono::minutes &time,
    const Offer &offer, const OfferManager &offer_manager) const {

  std::chrono::minutes proposed_start_time = time;
  std::chrono::minutes proposed_end_time = time + offer.duration;

  // Check if the proposed slot is within working hours (e.g., 09:00 - 17:00)
  std::chrono::minutes day_starts_at(9 * 60);
  std::chrono::minutes day_ends_at(17 * 60);

  if (proposed_start_time < day_starts_at || proposed_end_time > day_ends_at) {
    return false; // Outside working hours
  }

  auto slots = getAvailableSlots(date, offer_manager);
  if (slots.empty() &&
      (day_starts_at <
       day_ends_at)) { // if slots is empty but it's a valid working day
    // this condition means the whole day is available if no booked appointments
    // were found by getAvailableSlots however, getAvailableSlots should return
    // the [09:00, 17:00] slot if empty if slots is truly empty, it means no
    // availability. Let's rely on getAvailableSlots. If it's empty, it means no
    // slot.
  }

  for (const auto &slot : slots) {
    if (slot.start_time <= proposed_start_time &&
        slot.end_time >= proposed_end_time) {
      return true;
    }
  }

  return false;
}
