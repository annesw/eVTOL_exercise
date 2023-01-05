// chargers_and_queue.h

#ifndef CHARGERS_AND_QUEUE_H
#define CHARGERS_AND_QUEUE_H

#include "aircraft.h"

// remove //class Aircraft;
#define NUMBER_OF_AIRCRAFT 20

class Charger{

public:

enum ChargerState{
    empty,
    charging,
};

Charger();

ChargerState current_state;

};


class ChargerQueue{

public:

#define NUMBER_OF_CHARGERS 3

Charger chargers[NUMBER_OF_CHARGERS];

void put_aircraft_in_queue(Aircraft *aircraft_to_queue);

// The ChargeQueue does not need to keep track of seconds, but it will check if any
// Aircraft are waiting for chargers once a second.
void one_second_tick(void);

ChargerQueue();

void report_charger_free(int id);

private:

// For the charging queue, I am using a circular buffer of Aircraft pointers. The
// implementation is private so it can be changed to a different design in the future
// if needed.

// The queue size is one more than the number of airplanes so avoid extra logic to
// determine if the queue is empty or full when HEAD is equal to TAIL.
// We also avoid error handling when the queue is full in this case.
#define CHARGING_QUEUE_SIZE (NUMBER_OF_AIRCRAFT + 1)

int head; // Take aircraft off the head.
int tail; // Add aircraft at the tail. 
Aircraft* aircraft_queue[CHARGING_QUEUE_SIZE];

};


#endif // CHARGERS_AND_QUEUE_H
