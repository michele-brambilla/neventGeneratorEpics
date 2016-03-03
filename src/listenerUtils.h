#ifndef _LISTENERUTILS_H
#define _LISTENERUTILS_H
#include <iostream>
#include <type_traits>

#include "neventRequest.h"
/* #include "neventMonitor.h" */

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;


namespace utils {
  
  unsigned int getULong(std::string const& , PVStructure::shared_pointer const &,const std::string&);

  
  std::vector<uint64_t>& getArrayContent(std::string const &,
                                         PVStructure::shared_pointer const &,
                                         const std::string&,
                                         std::vector<uint64_t>&);
  


  template<class T>
    T& getContent(std::string const & channelName,
                  PVStructure::shared_pointer const & pv,
                  const std::string& field,
                  T& container) {
    
    PVField::shared_pointer v = pv->getSubField(field);
    if (v.get() == 0) {
      std::cerr << "no " << field << " field" << std::endl;
      return container;
    }
    
    
    Type valueType = v->getField()->getType();
    
    if (valueType == scalar) {
      std::cout << "scalar" << std::endl;
      (*container) = getULong(channelName,
                              pv,field);
      
      
    }
    else {
      std::cout << "scalarArray" << std::endl;
      getArrayContent(channelName, pv, field, container);
    }
    
    return container;
  }
  
  
  void do_something(Channel::shared_pointer const &,
                    shared_ptr<ChannelGetRequesterImpl> const&,
                    PVStructure::shared_pointer const&);
}
#endif //LISTENERUTILS_H
  
