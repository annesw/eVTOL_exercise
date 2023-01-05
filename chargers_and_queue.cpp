// chargers_and_queue.cpp

#include <iostream>
#include <random>
using namespace std;

#include "chargers_and_queue.h"
#include "aircraft.h"


Charger::Charger(){
   current_state = empty;
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
    aircraft_queue[tail] = aircraft_to_queue;
    tail++;
    if (tail >= CHARGING_QUEUE_SIZE) {
       tail = 0;
    }
    cout << " tail = " << tail << endl;
}

void ChargerQueue::one_second_tick(){
 int i;
 // Check if any of the chargers are free. Start charging any waiting aircraft if there is a free charger.
 for (i = 0; i < NUMBER_OF_CHARGERS; i++) {
     if (chargers[i].current_state == Charger::empty){
        //cout << "charger empty" << i << endl;
        //cout << "head " << head << "   tail " << tail << endl;
        if (head != tail) {
        // There is at least one aircraft in the queue.
            chargers[i].current_state = Charger::charging;
            aircraft_queue[head]->start_charging(i);
            aircraft_queue[head] = NULL;
            head++;
            if (head >= CHARGING_QUEUE_SIZE){
              head = 0;
            }
        } else {
            // We do not need to check the other chargers if there are no aircraft in the queue.
            break;
        }

     }

 }
  
}

void ChargerQueue::report_charger_free(int id){
   chargers[id].current_state = Charger::empty;
}
