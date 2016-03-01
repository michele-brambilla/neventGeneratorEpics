#ifndef _LISTENERUTILS_H
#define _LISTENERUTILS_H
#include <iostream>

#include "neventRequest.h"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;

static const double default_timeout = 0.1;

#define DEFAULT_REQUEST "field(value)"
#define DEFAULT_PROVIDER "pva"

double timeOut = default_timeout;
string request(DEFAULT_REQUEST);
string defaultProvider(DEFAULT_PROVIDER);
const string noAddress;


unsigned int getULong(std::string const& , PVStructure::shared_pointer const &,const std::string&);

std::vector<uint64_t>& getArrayContent(std::string const &,
                                       PVStructure::shared_pointer const &,
                                       const std::string&,
                                       std::vector<uint64_t>&);

void do_something(Channel::shared_pointer const &,
                  shared_ptr<ChannelGetRequesterImpl> const&,
                  PVStructure::shared_pointer const&);

#endif //LISTENERUTILS_H
