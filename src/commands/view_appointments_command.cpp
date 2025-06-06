#include "commands/view_appointments_command.h"
#include "appointment_manager.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm> // For std::sort
#include <sstream>   // For ostringstream

// Helper function to print a row, handling potential truncation
void printAppointmentRow(const std::string& client, const std::string& offer,
                         const std::string& date, const std::string& time,
                         int clientWidth, int offerWidth, int dateWidth, int timeWidth) {
    std::string displayClient = client;
    if (displayClient.length() > static_cast<size_t>(clientWidth)) {
        displayClient = displayClient.substr(0, clientWidth - 3) + "...";
    }
    std::string displayOffer = offer;
    if (displayOffer.length() > static_cast<size_t>(offerWidth)) {
        displayOffer = displayOffer.substr(0, offerWidth - 3) + "...";
    }

    std::cout << "| " << WHT << std::left << std::setw(clientWidth) << displayClient << CRESET << " "
              << "| " << WHT << std::left << std::setw(offerWidth) << displayOffer << CRESET << " "
              << "| " << WHT << std::left << std::setw(dateWidth) << date << CRESET << " "
              << "| " << WHT << std::left << std::setw(timeWidth) << time << CRESET << " |"
              << std::endl;
}


ViewAppointmentsCommand::ViewAppointmentsCommand()
    : Command({"view_appointments"}) {}

int ViewAppointmentsCommand::execute(std::vector<std::string> args) {
  AppointmentManager appointment_manager;
  auto appointments = appointment_manager.getEntries(); // Mutable copy

  if (appointments.empty()) {
    std::cout << COLOR("No appointments found.", YEL) << std::endl;
    return 0;
  }

  // Sort appointments: by date, then time, then client name
  std::sort(appointments.begin(), appointments.end(), [](const Appointment& a, const Appointment& b) {
    if (a.date != b.date) {
      return a.date < b.date;
    }
    if (a.time != b.time) {
      return a.time < b.time;
    }
    return a.client_name < b.client_name;
  });

  // Define column widths
  int clientNameWidth = 20;
  int offerNameWidth = 25;
  int dateWidth = 12; // "dd/mm/yyyy"
  int timeWidth = 7;  // "hh:mm"

  std::cout << COLOR("Scheduled Appointments (Sorted by Date, Time, Client):", BCYN) << std::endl;

  // Top Border
  std::cout << "+-" << std::string(clientNameWidth, '-') << "-+-"
            << std::string(offerNameWidth, '-') << "-+-"
            << std::string(dateWidth, '-') << "-+-"
            << std::string(timeWidth, '-') << "-+" << std::endl;

  // Header
  std::cout << "| " << BYEL << std::left << std::setw(clientNameWidth) << "Client Name" << CRESET << " "
            << "| " << BYEL << std::left << std::setw(offerNameWidth) << "Offer Name" << CRESET << " "
            << "| " << BYEL << std::left << std::setw(dateWidth) << "Date" << CRESET << " "
            << "| " << BYEL << std::left << std::setw(timeWidth) << "Time" << CRESET << " |"
            << std::endl;

  // Middle Border
  std::cout << "+-" << std::string(clientNameWidth, '-') << "-+-"
            << std::string(offerNameWidth, '-') << "-+-"
            << std::string(dateWidth, '-') << "-+-"
            << std::string(timeWidth, '-') << "-+" << std::endl;


  for (const auto &appointment : appointments) {
    std::chrono::hh_mm_ss<std::chrono::minutes> time_display(appointment.time);
    
    std::ostringstream date_stream;
    date_stream << std::setfill('0') << std::setw(2) << static_cast<unsigned>(appointment.date.day()) << "/"
                << std::setfill('0') << std::setw(2) << static_cast<unsigned>(appointment.date.month()) << "/"
                << static_cast<int>(appointment.date.year());
    std::string formatted_date = date_stream.str();

    std::ostringstream time_stream_display;
    time_stream_display << std::setfill('0') << std::setw(2) << time_display.hours().count() << ":"
                        << std::setfill('0') << std::setw(2) << time_display.minutes().count();
    std::string formatted_time_display = time_stream_display.str();

    printAppointmentRow(appointment.client_name, appointment.offer_name,
                        formatted_date, formatted_time_display,
                        clientNameWidth, offerNameWidth, dateWidth, timeWidth);
  }

  // Bottom Border
  std::cout << "+-" << std::string(clientNameWidth, '-') << "-+-"
            << std::string(offerNameWidth, '-') << "-+-"
            << std::string(dateWidth, '-') << "-+-"
            << std::string(timeWidth, '-') << "-+" << std::endl;

  return 0;
}
