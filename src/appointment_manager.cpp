#include <appointment_manager.h>
#include <sstream>

Appointment::Appointment(std::string client_name, std::string offer_name,
                         std::chrono::year_month_day date,
                         std::chrono::hh_mm_ss<std::chrono::minutes> time)
    : client_name{client_name}, offer_name{offer_name}, date{date}, time{time} {
}

std::vector<std::string>
AppointmentBuilder::serialize(const Appointment &entry) const {
  std::vector<std::string> values;
  values.push_back(entry.client_name);
  values.push_back(entry.offer_name);
  values.push_back(std::format("{:2}/{:2}/{:4}", entry.date.day(),
                               entry.date.month(), entry.date.year()));
  values.push_back(
      std::format("{:2}:{:2}", entry.time.hours(), entry.time.minutes()));

  return values;
}

Appointment
AppointmentBuilder::deserialize(const std::vector<std::string> &values) const {
  if (values.size() != 4) {
    throw std::invalid_argument("Expected 4 values for Appointment");
  }

  std::istringstream is(values[2]);

  std::chrono::year_month_day date;
  is >> std::chrono::parse("%d/%m/%Y", date);

  std::chrono::minutes minutes;
  is >> std::chrono::parse("%H:%M", minutes);

  return Appointment(values[0], values[1], date,
                     std::chrono::hh_mm_ss<std::chrono::minutes>(minutes));
}

AppointmentManager::AppointmentManager()
    : CsvFile<Appointment, AppointmentBuilder>("./data/appointments.csv") {}
