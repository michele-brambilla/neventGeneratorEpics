#ifndef EVENTPRODUCER_H
#define EVENTPRODUCER_H

#include <queue>
#include <random>
#include <string>
#include <iostream>

#include "nevent.h"
#include "nexus2event.h"


struct EventProducer {
  EventProducer(const double&);
  void GenerateEvents(char*);

  nevent GetEvent(const int);
  unsigned int GetEventCount();
  const pNEventArray get_ptr() { return nEventData; };
  
private:
  const double multiplier;
  unsigned int count;
  pNEventArray nEventData;
};

#endif  /* EVENTPRODUCER_H */
