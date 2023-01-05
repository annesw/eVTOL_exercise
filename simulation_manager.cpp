// simulation_manager.cpp
#include <iostream>
#include <random>
using namespace std;

#include "aircraft.h"
#include "simulation_manager.h"
#include "chargers_and_queue.h"

extern ChargerQueue* g_charger_queue_ptr;

// Initial data for different aircraft types. 
typedef struct _aircraft_info {
Aircraft::Manufacturer manufacturer;
int cruise_speed_mph;
int time_to_charge_seconds;
int flight_time_from_full_charge_seconds;
int passenger_count;
float probablity_of_fault_per_hour;
} aircraft_info;

aircraft_info initial_aircraft_data[] = {
{Aircraft::Alpha, 120, 2160, 6000, 4, 0.25},
{Aircraft::Bravo, 100, 720, 2400, 5, 0.10},
{Aircraft::Charlie, 160, 2880, 2250, 3, 0.05},
{Aircraft::Delta, 90, 2232, 6000, 2, 0.22},
{Aircraft::Echo, 30, 1080, 29508, 2, 0.61}
};



void SimulationManager::create_aircraft(void){
    int i, aircraft_type;
    Aircraft A1;
    cout << "creating aircraft" << endl;
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, 4); // define the range
    for (i = 0 ; i < NUMBER_OF_AIRCRAFT; i++) {
        // randomly generate which type of plane.
        aircraft_type = distr(gen);
        cout << aircraft_type << ' '; // generate numbers
        all_aircraft[i] = A1(
            initial_aircraft_data[aircraft_type].manufacturer,
            initial_aircraft_data[aircraft_type].cruise_speed_mph,
            initial_aircraft_data[aircraft_type].time_to_charge_seconds,
            initial_aircraft_data[aircraft_type].flight_time_from_full_charge_seconds,
            initial_aircraft_data[aircraft_type].passenger_count,
            initial_aircraft_data[aircraft_type].probablity_of_fault_per_hour);
            cout << Aircraft::get_manufacturer_string(all_aircraft[i].get_manufacturer()) << endl;
            // The id will happen to match the index, but they are only intended to be an
            // unique identifier, not an index.
            all_aircraft[i].set_aircraft_id(i); 
    }
    cout << endl;
}

void SimulationManager::run_simulation(void){
  int second_count;
  int aircraft_index;

  // Give aircraft access to the charging queue.
  g_charger_queue_ptr = &charger_queue;

  for (second_count = 0; second_count <= SECONDS_IN_SIMULATION; second_count++) { // Go one extra second because we start on 1;

    for (aircraft_index = 0 ; aircraft_index < NUMBER_OF_AIRCRAFT ; aircraft_index++){
        all_aircraft[aircraft_index].one_second_tick();
    } 
    charger_queue.one_second_tick(); 
  }
}
