// chargers_and_queue.cpp

#include <iostream>
#include <random>
using namespace std;

#include "chargers_and_queue.h"
#include "aircraft.h"


Charger::Charger(){
   current_state = empty;
   charging_aircraft = NULL;
}

// This will attach an aircraft to a charger and start charging.
// The aircraft keeps track of the charging time.
// To Do: This does not do any error checking. In a real life
// situation, you would check if the charger was free, or return 
// an error code. The aircraft would put itself back into the charging
// queue if there was an error.
void Charger::start_charging_aircraft(Aircraft *aircraft_to_charge){
   current_state = charging;
   charging_aircraft = aircraft_to_charge;
}

// This will change the state to empty. It is a design choice to not have the Charger report to the ChargerQueue when
// it is empty, instead the ChargerQueue will check for empty chargers if there are queued airplanes. It could have also
// been designed so that the Charger notified the ChargerQueue.
// To Do: This does not do any error checking. For example, it could pass in the aircraft that is detaching and checking
// that it is the same as the current charging_airplane, this would need a strategy for what to do in that error situation.
void Charger::aircraft_report_done_and_detach(void){
   current_state = empty;
   charging_aircraft = NULL;
}


ChargerQueue::ChargerQueue(void){
  int i;
  head = 0;
  tail = 0;

  for (i = 0; i < CHARGING_QUEUE_SIZE; i++){
    aircraft_queue[i] = NULL;
  }
}

void ChargerQueue::put_aircraft_in_queue(Aircraft *aircraft_to_queue){
    cout << "put aircraft_in_queue" << endl;
    aircraft_queue[tail] = aircraft_to_queue;
    cout << "A" << endl;
    tail++;
    if (tail >= CHARGING_QUEUE_SIZE) {
       tail = 0;
    }
    cout << " tail = " << tail << endl;
}

void ChargerQueue::one_second_tick(){
 int i;
 cout << "charger tick" << endl;
 // Check if any of the chargers are free. Start charging any waiting aircraft if there is a free charger.
 for (i = 0; i < NUMBER_OF_CHARGERS; i++) {
     if (chargers[i].current_state == Charger::empty){
        //cout << "charger empty" << i << endl;
        //cout << "head " << head << "   tail " << tail << endl;
        if (head != tail) {
        // There is at least one aircraft in the queue.
            cout << "at least one aircraft in the queue." << endl;
            aircraft_queue[head]->start_charging(&chargers[i]);
            aircraft_queue[head] = NULL;
            head++;
            if (head >= CHARGING_QUEUE_SIZE){
              head = 0;
            }
        } else {
            cout << "no aircraft in queue" << endl;
            // We do not need to check the other chargers if there are no aircraft in the queue.
            break;
        }

     }

 }
  
}
