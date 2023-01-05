// simulation_manager.h


#ifndef SIMULATION_MANAGER_H
#define SIMULATION_MANAGER_H

#include "aircraft.h"
#include "chargers_and_queue.h"

#define NUMBER_OF_AIRCRAFT 20
#define SECONDS_IN_SIMULATION 10800  // 3 hours

class FaultGenerator{
public:
   FaultGenerator();
   bool did_a_fault_occur(double probablity);
};

// The SimulationManager will create the aircraft, start the TimeManager and
// have the StatisticReporter print out the results.
// To Do: May want to make this a singlton, so that there is only SimulationManager.
class SimulationManager {

public:

   // This is an array because the current scenario is relatively simple. In a more complicated situation, for example if
   // the aircrafts are diffent classes or there are variable number of aircraft, we might want to allocate the aircraft 
   // at runtime and have an array of pointers, a linked list of pointers or some other collection of pointers. 
   Aircraft all_aircraft[NUMBER_OF_AIRCRAFT];
   ChargerQueue charger_queue;
   FaultGenerator fault_generator;

   void create_aircraft(void);
   
   void run_simulation(void);

   void report_on_data(void);
};




#endif // SIMULATION_MANAGER_H
