#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
//#include <string.h>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/standardField.h>
#include <pv/standardPVField.h>

#define epicsExportSharedSymbols

#include <pv/channelProviderLocal.h>
#include <pv/traceRecord.h>

#include "nevent.h"

#include <neventDataRecord.h>


using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::nevent;

static const double multiplier = 1.;



neventDataRecord::shared_pointer neventDataRecord::create(const std::string &recordName)
{
    // Structure is:
    //  unsigned long count;
    //  long *detectorID;
    //  int *nTimeStamp;
    
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardPVFieldPtr standardPVField = getStandardPVField();
    
    epics::pvData::PVStructurePtr pvStructure = pvDataCreate->createPVStructure(
        fieldCreate->createFieldBuilder()->
        add("timeStamp", standardField->timeStamp()) ->
        add("count", pvULong) ->
        addArray("detectorId", pvLong)->
        addArray("nTimeStamp", pvInt)->
        createStructure()
    );
    
    neventDataRecord::shared_pointer pvRecord(new neventDataRecord(recordName, pvStructure));
	
    if (pvRecord && !pvRecord->init())
	        pvRecord.reset();

    return pvRecord;    
}

neventDataRecord::neventDataRecord(const std::string &recordName, const epics::pvData::PVStructurePtr &pvStructure)
  : epics::pvDatabase::PVRecord(recordName, pvStructure)
{
    std::cout << "Constructor" << std::endl;
    // reg = new RandomEventGenerator();
    prod = new EventProducer(multiplier);
}


bool neventDataRecord::init()
{
  initPVRecord();
  if (!pvTimeStamp.attach(getPVStructure()->getSubField("timeStamp")))
    return false;
    
  nCount = getPVStructure()->getSubField<PVULong>("count");
  if (nCount.get() == NULL)
    return false;
    
  nDetectorId = getPVStructure()->getSubField<PVLongArray>("detectorId");
  if (nDetectorId.get() == NULL)
    return false;
    
  nTimeStamp = getPVStructure()->getSubField<PVIntArray>("nTimeStamp");
  if (nTimeStamp.get() == NULL)
    return false;

  return true;
}

void neventDataRecord::update(char* filename) {

  prod->GenerateEvents(filename);
  std::cout << "eventID = " << eventID
            << "generatorID = " << *(prod->getID())
            << std::endl;

  if(eventID % 140 == 0) {

    oldID = *(prod->getID());

    if( oldID - eventID > 0) {
      std::cout << "Lost " << oldID - eventID
                << " events"<< std::endl;

      // reset counter
      eventID = 0;
      *(prod->getID()) = 0;
    }
  }
  ++eventID;

  
  // Need to put queued events into arrays
  PVLongArray::svector id_data;
  PVIntArray::svector ts_data;
  unsigned int size = prod->GetEventCount();

  // nDetectorId->setCapacity(size);
  // nTimeStamp->setCapacity(size);
  //  memcpy(nDetectorId->getPtrSelf(),prod->get_ptr(),size*sizeof(int64_t));

  struct nevent ev;
  for (unsigned int i = 0; i < size; ++i) {
    ev = prod->GetEvent(i);
    id_data.push_back(ev.detectorID);
    ts_data.push_back(ev.timeStamp);
  }
  
  beginGroupPut();
  
  nCount->put(size);
  nDetectorId->replace(freeze(id_data));
  nTimeStamp->replace(freeze(ts_data));
  
  // EPICS timestamp updated last
  timeStamp.getCurrent();
  pvTimeStamp.set(timeStamp);

  // std::cout << clock() << std::endl;
  endGroupPut();
  
}
