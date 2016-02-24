#include <random>
#include <algorithm>
#include <type_traits>


#include "event_producer.h"

EventProducer::EventProducer(const double& mult) : multiplier(mult) { }

void EventProducer::GenerateEvents(char* filename) {
  static int c = 1;
  if(c) {
    c--;
    nEventData = loadNeXus2Events(filename);
  }
  count = nEventData->count;
}

nevent EventProducer::GetEvent(const int eventId) {
  //  count = eventId;
  struct nevent ev;
  ev.detectorID = nEventData->detectorID[eventId];
  ev.timeStamp = nEventData->timeStamp[eventId];
  // std::cout << eventId << " : "
  //           << "( "  << ev.detectorID
  //           << " , " << ev.timeStamp
  //           << " )" << std::endl;
  return ev;
}

unsigned int EventProducer::GetEventCount()
{
  return count;
}
