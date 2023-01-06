// aircraft.h

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <string>

// Class for each individual aircraft. I considered making the Aircraft class
// a base class and making each manufacturing class a derived class. I ruled this
// out at this point because, at this point, all the aircrafts function the same even
// if their numeric values are different.

// I have converted the relevent values from the instructions to seconds. This was
// an arbitrary choice just because it is easier to work with whole numbers for the
// exercise. 
//
// time_to_charge_seconds is:
//     Time to Charge hours * 60 minutes/hour * 60 seconds/minute
//
// flight_seconds_per_charge is done in 2 steps:
// miles per charge is:
//     Battery Capacity kWh / Energy use at Cruise (kWh/mile)
// flight seconds per charge is:
//     miles per charge * 60 minutes/hour * 60 seconds/minute
//     divided by:
//     Cruise Speed (mph)
//

class Aircraft{

public:

enum Manufacturer {
  Alpha = 0,
  Bravo = 1,
  Charlie = 2,
  Delta = 3,
  Echo = 4,
  Uninitialized = 5
};


protected:

enum AircraftStates{
   before_simulation,
   flying,
   waiting_to_charge,
   charging,
   after_simulation, 
};

AircraftStates current_state;
Manufacturer manufacturer;
int cruise_speed_mph;
int time_to_charge_seconds;
int flight_seconds_per_charge;
int passenger_count;
float probablity_of_fault_per_hour;
int total_faults;
int total_seconds;
int total_seconds_flying;
int flight_start_second;
int charge_start_second;
int aircraft_id;
int current_charger_id;
int number_of_flights;
int number_of_charges;
int total_seconds_charging;
 
public:
Aircraft();
Aircraft operator()(Manufacturer manu,
int cruise_speed,
int seconds_to_charge, 
int flight_time_seconds,
int passengers,
float probablity_of_fault);

Aircraft::Manufacturer get_manufacturer(void);

static std::string get_manufacturer_string(Aircraft::Manufacturer);

void one_second_tick(void);

void start_charging(int charger_id);

void set_aircraft_id(int id);
int get_aircraft_id(void);
void put_aircraft_in_charging_queue(void);
void aircraft_report_done_charging_and_detach(void);

friend class SimulationManager;

};

#endif // AIRCRAFT_H
