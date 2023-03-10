#include "aircraft.h"
#include "chargers_and_queue.h"
#include "simulation_manager.h"

#include <iostream>
#include <string>
using namespace std;

extern ChargerQueue* g_charger_queue_ptr;
extern FaultGenerator* g_fault_generator;

Aircraft::Aircraft(){
  manufacturer = Uninitialized;
  cruise_speed_mph = 1;
  time_to_charge_seconds = 1;
  flight_seconds_per_charge = 1;
  passenger_count = 1;
  probablity_of_fault_per_hour = 0.1;
  current_state = before_simulation;
  total_faults = 0;
  total_seconds = 0;
  total_seconds_flying = 0;
  number_of_flights = 0;
  number_of_charges = 0;
  total_seconds_charging = 0;
};

Aircraft Aircraft::operator()(Manufacturer manu,
  int cruise_speed,
  int seconds_to_charge, 
  int flight_time_seconds,
  int passengers,
  float probablity_of_fault){

  Aircraft A;
  A.manufacturer = manu;
  A.cruise_speed_mph = cruise_speed;
  A.time_to_charge_seconds = seconds_to_charge;
  A.flight_seconds_per_charge = flight_time_seconds;
  A.passenger_count = passengers;
  A.probablity_of_fault_per_hour = probablity_of_fault;
  return A;
}

Aircraft::Manufacturer Aircraft::get_manufacturer(void){
    return manufacturer;
}

string Aircraft::get_manufacturer_string(Aircraft::Manufacturer manufacturer_in){
    string manufacturer_str;
    switch (manufacturer_in) {
        case Aircraft::Alpha :
          manufacturer_str = "Alpha";
          break;
        case Aircraft::Bravo :
          manufacturer_str = "Bravo";
          break;
        case Aircraft::Charlie :
          manufacturer_str = "Charlie";
          break;
        case Aircraft::Delta :
          manufacturer_str = "Delta";
          break;
        case Aircraft::Echo :
          manufacturer_str = "Echo";
          break;
        default:
           manufacturer_str = "unknown";
    }
    return manufacturer_str;
}

void Aircraft::one_second_tick(void){
  total_seconds++;

  // Check for faults once a minute.
  if ((total_seconds % 60) == 0){
     if (g_fault_generator->did_a_fault_occur(probablity_of_fault_per_hour / 60)){
         total_faults++;
         cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
         cout << " FAULT at second: " << total_seconds << endl;
     }
  }
  
  switch (current_state){
    case (before_simulation):
      // Assume that the first tick is starting the simulation.
      current_state = flying; 
      flight_start_second = total_seconds;
      total_seconds_flying++;
      number_of_flights++;
      cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
      cout << " flying at second: " << total_seconds << endl;
      break;
    case (flying):
      // Check if we are past the total number of seconds.
      if (total_seconds > (flight_start_second + flight_seconds_per_charge)) {
         cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
         cout << " done flying at second: " << total_seconds << endl;
         // Add ourselves to the queue.
         current_state = waiting_to_charge;
         put_aircraft_in_charging_queue();
         cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
         cout << " queueing at second: " << total_seconds << endl;
      } else {
         // Track total flying seconds.
         total_seconds_flying++;
      } 
      break;
    case (waiting_to_charge):
      // Do nothing until we are put into a charger.

      break;
    case (charging):
      // Check if we are done charging.
      if (total_seconds > (charge_start_second + time_to_charge_seconds)) {
            // Detach from the charger
            aircraft_report_done_charging_and_detach();
            // start flying
            current_state = flying; 
            flight_start_second = total_seconds;
            total_seconds_flying++;
            number_of_flights++;
            cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
            cout << " flying at second: " << total_seconds << endl;
      } else {
          total_seconds_charging++;
      }
      break;
    case (after_simulation):
      break;
  } 
}


void Aircraft::set_aircraft_id(int id){
  aircraft_id = id;
}

int Aircraft::get_aircraft_id(void){
  return aircraft_id;
}


void Aircraft::start_charging(int charger_id){
  current_charger_id = charger_id;
  charge_start_second = total_seconds;
  number_of_charges++;
  cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
  cout << " started charging at second: " << total_seconds;
  cout << " charger id " << current_charger_id << endl;
  current_state = charging;
}


void Aircraft::put_aircraft_in_charging_queue(void){
    g_charger_queue_ptr->put_aircraft_in_queue(this);
}

void Aircraft::aircraft_report_done_charging_and_detach(void){
  cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer);
  cout << " releasing charger at second: " << total_seconds;
  cout << " charger id " << current_charger_id << endl;
    g_charger_queue_ptr->report_charger_free(current_charger_id);
}
