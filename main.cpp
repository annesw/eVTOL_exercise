#include <string.h>
#include <iostream>
using namespace std;

#include "aircraft.h"
#include "simulation_manager.h"
#include "chargers_and_queue.h"

ChargerQueue* g_charger_queue_ptr;

void run_tests(){
cout << "Running tests." << endl;
        Aircraft aircraft1;
        cout << "aircraft1 manufacturer " << Aircraft::get_manufacturer_string(Aircraft::Bravo) << endl;
        cout << " RAND_MAX " << RAND_MAX << endl;

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
        cout << "simulation done" << endl;
	return 0;
}

