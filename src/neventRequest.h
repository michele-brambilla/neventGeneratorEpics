#ifndef _NEVENTREQUEST_H
#define _NEVENTREQUEST_H

#include <iostream>

#include <epicsExport.h>
#include <epicsExit.h>
#include <pv/event.h>
#include <pv/caProvider.h>
#include <pv/logger.h>
#include <pv/clientFactory.h>

#include "uri.h"

struct RequesterImpl : public epics::pvData::Requester {
  RequesterImpl(std::string const&);
  std::string getRequesterName();
  void message(std::string const&, epics::pvData::MessageType);
private:
  std::string requesterName;
};


class ChannelRequesterImpl :
public epics::pvAccess::ChannelRequester {
 private:
  epics::pvData::Event m_event;
  bool printOnlyErrors;
  bool showDisconnectMsg;

 public:
  
  ChannelRequesterImpl(bool _printOnlyErrors = false);
  
  virtual std::string getRequesterName();
  virtual void message(std::string const &, epics::pvData::MessageType);
  
  virtual void channelCreated(const epics::pvData::Status& status,
                              epics::pvAccess::Channel::shared_pointer const &);
  virtual void channelStateChange(epics::pvAccess::Channel::shared_pointer const &,
                                  epics::pvAccess::Channel::ConnectionState);
    
  bool waitUntilConnected(double timeOut);
  void showDisconnectMessage(bool show = true);
};

#endif //NEVENTREQUEST_H
