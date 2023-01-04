#include "aircraft.h"

#include <iostream>
#include <string>
using namespace std;
 

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
seconds_flying_this_charge = 0;
seconds_charging_current_charge = 0;
charger_being_used = NULL;
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

void Aircraft::set_starting_values(ChargerQueue *charger_queue){
   // Store the charging queue. 
   charger_queue_to_use = charger_queue;
}

void Aircraft::one_second_tick(void){
  total_seconds++;
  if (aircraft_id == 1){
    cout << "seconds " << total_seconds << endl;
  }
  switch (current_state){
    case (before_simulation):
      // Assume that the first tick is starting the simulation.
      current_state = flying; 
      flight_start_second = total_seconds;
      total_seconds_flying++;
      cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer) << " flying" << endl;
      break;
    case (flying):
      // Check if we are past the total number of seconds.
      if (total_seconds > (flight_start_second + flight_seconds_per_charge)) {
         cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer) << " queueing" << endl;
         // Add ourselves to the queue.
         current_state = waiting_to_charge;
         charger_queue_to_use->put_aircraft_in_queue(ptr_to_myself);
         cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer) << " queueing" << endl;
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
            charger_being_used->aircraft_report_done_and_detach();
            charger_being_used = NULL;
            // start flying
            current_state = flying; 
            flight_start_second = total_seconds;
            total_seconds_flying++;
            cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer) << " flying" << endl;
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


void Aircraft::start_charging(Charger * charger_to_use){
  charger_being_used = charger_to_use;
  charge_start_second = total_seconds;
  charger_being_used->start_charging_aircraft(this);
  cout << "Aircraft " << aircraft_id << " " << get_manufacturer_string(manufacturer) << " charging" << endl;
   current_state = charging;
}


void Aircraft::set_pointer_to_myself(Aircraft * my_pointer){
  ptr_to_myself =  my_pointer;
}
