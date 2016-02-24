#ifndef EVENTPRODUCER_H
#define EVENTPRODUCER_H

#include <queue>
#include <random>
#include <string>
#include <iostream>

#include "nevent.h"
#include "nexus2event.h"

static const double frequency = 10;

struct EventProducer {
  EventProducer(const double&);
  unsigned int next() { return (++eID);  }

  void GenerateEvents(char*);

  nevent GetEvent(const int);
  unsigned int GetEventCount();
  const pNEventArray get_ptr() { return nEventData; };
  unsigned int* getID() { return &eID; }
  
private:
  const double multiplier;
  unsigned int count;
  unsigned int eID;
  pNEventArray nEventData;
};



void keep_pulling(EventProducer*);

#endif  /* EVENTPRODUCER_H */
