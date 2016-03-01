#include <iostream>

#include "neventRequest.h"
#include "listenerUtils.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;

int main (int argc, char *argv[]) {

  int nPvs = argc - optind;       /* Remaining arg list are PV names */
  vector<string> pvs;     /* Array of PV structures */

  // copy PV names from command line
  for (int n = 0; optind < argc; n++, optind++) {
    pvs.push_back(argv[optind]);
  }
  
  std::cout << std::boolalpha;
  bool allOK = true;

  PVStructure::shared_pointer pvRequest;
  {
    //    Requester::shared_pointer requester(new RequesterImpl("neventListenerEpics"));
    
    pvRequest = CreateRequest::create()->createRequest(request);
    if(pvRequest.get()==NULL) {
      fprintf(stderr, "failed to parse request string\n");
      return 1;
    }
  }
  std::vector<std::string> pvNames;
  std::vector<std::string> pvAddresses;
  std::vector<std::string> providerNames;
  
  for (int n = 0; n < nPvs; n++) {
    URI uri;
    bool validURI = URI::parse(pvs[n], uri);
    
    std::string providerName(defaultProvider);
    std::string pvName(pvs[n]);
    std::string address(noAddress);
    bool usingDefaultProvider = true;
    if (validURI) {
      if (uri.path.length() <= 1) {
        std::cerr << "invalid URI '" << pvs[n] << "', empty path" << std::endl;
        return 1;
      }
      providerName = uri.protocol;
      pvName = uri.path.substr(1);
      address = uri.host;
      usingDefaultProvider = false;
    }
    
    if ((providerName != "pva") && (providerName != "ca")) {
      std::cerr << "invalid "
                << (usingDefaultProvider ? "default provider" : "URI scheme")
                << " '" << providerName 
                << "', only 'pva' and 'ca' are supported" << std::endl;
      return -1;
    }
    pvNames.push_back(pvName);
    pvAddresses.push_back(address);
    providerNames.push_back(providerName);
    
    std:: cout << "pvNames "       << pvNames[n]       << "\n"
               << "pvAddresses "   << pvAddresses[n]   << "\n"
               << "providerNames " << providerNames[n] << "\n"
               << std::endl;
  }
  
  // start both the pva and ca client providers
  ClientFactory::start();
  epics::pvAccess::ca::CAClientFactory::start();
  
  // first connect to all, this allows resource (e.g. TCP connection) sharing
  vector<Channel::shared_pointer> channels(nPvs);
  for (int n = 0; n < nPvs; n++) {
    shared_ptr<ChannelRequesterImpl> channelRequesterImpl(new ChannelRequesterImpl(true));
    if (pvAddresses[n].empty())
      channels[n] = getChannelProviderRegistry()->getProvider(
                                                              providerNames[n])->createChannel(pvNames[n],
                                                                                               channelRequesterImpl);
    else
      channels[n] = getChannelProviderRegistry()->getProvider(
                                                              providerNames[n])->createChannel(pvNames[n],
                                                                                               channelRequesterImpl,
                                                                                               ChannelProvider::PRIORITY_DEFAULT,
                                                                                               pvAddresses[n]);
  }

  // for now a simple iterating sync implementation, guarantees order
  for (int n = 0; n < nPvs; n++) {
    Channel::shared_pointer channel = channels[n];
    shared_ptr<ChannelRequesterImpl> channelRequesterImpl = dynamic_pointer_cast<ChannelRequesterImpl>(channel->getChannelRequester());
      
    if (channelRequesterImpl->waitUntilConnected(timeOut)) {
      shared_ptr<GetFieldRequesterImpl> getFieldRequesterImpl;
        
      // probe for value field
      getFieldRequesterImpl.reset(new GetFieldRequesterImpl(channel));
      // get all to be immune to bad clients not supporting selective getField request
      channel->getField(getFieldRequesterImpl, "");
        
        
      if (getFieldRequesterImpl.get() == 0 ||
          getFieldRequesterImpl->waitUntilFieldGet(timeOut)) {
        // check probe
        if (getFieldRequesterImpl.get()) {
          Structure::const_shared_pointer structure =
            dynamic_pointer_cast<const Structure>(getFieldRequesterImpl->getField());
          if (structure.get() == 0 || structure->getField("value").get() == 0) {
            // fallback to structure
            pvRequest = CreateRequest::create()->createRequest("field()");
          }
        }
      }


      int oldPulseID = -1;
      
      std::vector<uint64_t> detectorID, timestamp;
      
      int pulseID = -1, pulseCount = 0;
      uint64_t nCount;
      
      int statTime = time(NULL);

      while(true) {
        shared_ptr<ChannelGetRequesterImpl> getRequesterImpl(new ChannelGetRequesterImpl(channel->getChannelName()));
        while( oldPulseID == pulseID ) {
          
          
          
          
          ChannelGet::shared_pointer channelGet = channel->createChannelGet(getRequesterImpl,
                                                                            pvRequest);
          allOK &= getRequesterImpl->waitUntilGet(timeOut);
          
        // do_something(channel,getRequesterImpl,pvRequest);
        
          
          pulseID = getULong(channel->getChannelName(),
                             getRequesterImpl->getPVStructure(),
                             "eventTag");
          //          std::cout << oldPulseID << "\t" << pulseID << std::endl;
                 
        }
        if(pulseID - oldPulseID > 1) {
          printf("Missed pulse at pulseID %u\n", pulseID);
        }
        oldPulseID = pulseID;
        nCount = getULong(channel->getChannelName(),
                          getRequesterImpl->getPVStructure(),"count");
        getArrayContent(channel->getChannelName(),
                        getRequesterImpl->getPVStructure(),
                        "detectorId", detectorID);
        getArrayContent(channel->getChannelName(),
                        getRequesterImpl->getPVStructure(),
                        "nTimeStamp", detectorID);

        pulseCount++;
    
        if(time(NULL) >= statTime + 10) {
      
          printf("Received %f MB/sec , %f n* 10^6/sec, %u pulses\n", pulseCount*2*nCount*sizeof(uint64_t)/(1024.*1024.*10.), pulseCount*nCount/1.e6, pulseCount);
  
          pulseCount = 0;
          nCount = 0;
          statTime = time(NULL);
      
        }
    
            
      }
      
    }   
    
  }
    

    epics::pvAccess::ca::CAClientFactory::stop();
    ClientFactory::stop();
    //       epicsThreadSleep ( 3.0 );
    
    return allOK ? 0 : 1;
    return 0;
  }
