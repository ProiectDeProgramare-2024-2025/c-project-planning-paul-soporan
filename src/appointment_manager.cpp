#include <appointment_manager.h>
#include <sstream>

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

AppointmentManager::AppointmentManager() : CsvFile("./data/appointments.csv") {}
