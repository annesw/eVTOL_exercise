// simulation_manager.cpp
#include <iostream>
#include <random>
#include <cstdlib>
using namespace std;

#include "aircraft.h"
#include "simulation_manager.h"
#include "chargers_and_queue.h"

#define NUMBER_OF_AIRCRAFT_MANUFACTURERS 5

extern ChargerQueue* g_charger_queue_ptr;
extern FaultGenerator* g_fault_generator;

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
    cout << "Creating Aircraft" << endl;
    random_device rd; // obtain a random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(0, 4); // define the range
    for (i = 0 ; i < NUMBER_OF_AIRCRAFT; i++) {
        // randomly generate which type of plane.
        aircraft_type = distr(gen);
        cout << i << " ";
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

  cout << "Simulation Starting" << endl;
  // Give aircraft access to the charging queue.
  g_charger_queue_ptr = &charger_queue;
  g_fault_generator = &fault_generator;

  for (second_count = 0; second_count <= SECONDS_IN_SIMULATION; second_count++) { // Go one extra second because we start on 1;

    for (aircraft_index = 0 ; aircraft_index < NUMBER_OF_AIRCRAFT ; aircraft_index++){
        all_aircraft[aircraft_index].one_second_tick();
    } 
    charger_queue.one_second_tick(); 
  }
}

typedef struct _SummaryData {
  int type;                    
  int total_aircraft; 
  int total_flight_seconds;
  int total_number_of_flights;
  int total_charging_seconds;
  int total_number_of_charges;
  int cruise_speed;
  int total_faults;
  int passenger_count;
} SummaryData;


void print_data_for_aircraft_type(SummaryData * summary_data){
  float average_time_per_flight_hours;
  float average_distance_per_flight_in_miles;
  float average_time_charging_per_session_in_hours;
  float total_passenger_miles;
  cout << endl << "Results for manufacturer " << Aircraft::get_manufacturer_string((Aircraft::Manufacturer) summary_data->type) << endl;
  cout << "Total fight time: " << (float) summary_data->total_flight_seconds / 3600.0 << " miles" << endl;

  average_time_per_flight_hours = ((float)summary_data->total_flight_seconds / 3600.0) / (float) summary_data->total_number_of_flights;
  cout << "Average time per flight: " << average_time_per_flight_hours << " hours" << endl;

  average_distance_per_flight_in_miles = average_time_per_flight_hours * summary_data->cruise_speed;
  cout << "Average distances traveled per_flight: " << average_distance_per_flight_in_miles << " miles" << endl;

  average_time_charging_per_session_in_hours = ((float) summary_data->total_charging_seconds / 3600.0) / (float) summary_data->total_number_of_charges;
  cout << "Average time charging per charge session: " << average_time_charging_per_session_in_hours << endl;
  cout << "Total number of faults: " << summary_data->total_faults << endl;

  total_passenger_miles = ((float) summary_data->total_flight_seconds / 3600.0) * summary_data->passenger_count;
  cout << "Total passenger_miles: " << total_passenger_miles << endl;
}

void SimulationManager::report_on_data(void){
  int i, j;
  SummaryData summary_data; 
  for (i = 0; i < NUMBER_OF_AIRCRAFT_MANUFACTURERS ; i++){
  // Gather data.
     summary_data.type = i;
     summary_data.total_aircraft = 5;
     summary_data.total_flight_seconds = 300;
     summary_data.total_number_of_flights =  5;
     summary_data.total_charging_seconds = 150;
     summary_data.total_number_of_charges = 3;
     summary_data.cruise_speed = 150;
     summary_data.total_faults = 30;
     summary_data.passenger_count = 4;

  // Print data.
      print_data_for_aircraft_type(&summary_data);
  }

}

FaultGenerator::FaultGenerator(){
  srand (static_cast <unsigned> (time(0)));
}

bool FaultGenerator::did_a_fault_occur(double probablity){
   double random_between_zero_and_one;
   random_between_zero_and_one =  rand() / (RAND_MAX + 1.0);
   return (random_between_zero_and_one < probablity); 
}
