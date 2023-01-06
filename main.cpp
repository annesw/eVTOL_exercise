#include <string.h>
#include <iostream>
using namespace std;

#include "aircraft.h"
#include "simulation_manager.h"
#include "chargers_and_queue.h"

ChargerQueue* g_charger_queue_ptr;
FaultGenerator* g_fault_generator;

void test_fault_generator(){
  int i, interations;
  int number_of_faults;
  double probability_value;
  FaultGenerator fault_generator;
  cout << endl << "Testing Fault Generator:" << endl;

  // To Do: This could be turned into a subroutine and run many times with
  // different values.
  number_of_faults = 0;
  probability_value = 0.5;
  interations = 100;
  for (i = 0; i < interations; i++){
      if (fault_generator.did_a_fault_occur(probability_value)){
          number_of_faults++;
      }
  }
  cout << "iterations:" << interations << " probablity: " << probability_value;
  cout << " number of faults: " << number_of_faults << endl; 
  cout << "Expecting a result close to 50." << endl;

  number_of_faults = 0;
  probability_value = 0.25 / 60.0;
  interations = 100 * 60;
  for (i = 0; i < interations; i++){
      if (fault_generator.did_a_fault_occur(probability_value)){
          number_of_faults++;
      }
  }
  cout << "iterations:" << interations << " probablity: " << probability_value;
  cout << " number of faults: " << number_of_faults << endl; 
  cout << "Expecting a result close to 25." << endl;

}



void run_tests(){
  cout << "Running Tests" << endl;
  cout << endl << "Testing getting the aircraft type as a string. Using Bravo." << endl;
  cout << "aircraft manufacturer " << Aircraft::get_manufacturer_string(Aircraft::Bravo) << endl;
  test_fault_generator();

}

int main(int argc, char** argv){
        if (argc > 1){
           if(strcmp(argv[1], "tests") == 0){
               run_tests();
               exit (0);
           }
        }
        Aircraft aircraft1;
        SimulationManager sim_manager;
        sim_manager.create_aircraft();
        sim_manager.run_simulation();
        cout << endl << "Simulation Done" << endl << endl;
        sim_manager.report_on_data();
	return 0;
}

