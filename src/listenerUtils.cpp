#include "listenerUtils.h"
#include <memory>


unsigned int getULong(std::string const & channelName,
                      PVStructure::shared_pointer const & pv,
                      const std::string& fieldname) {
  
  PVField::shared_pointer v = pv->getSubField(fieldname);
  if (v.get() == 0) {
    std::cerr << "no " << fieldname << " field" << std::endl;
    return 1;
  }
    
  return pv->getSubField<PVULong>(fieldname)->get();
}


std::vector<uint64_t>& getArrayContent(std::string const & channelName,
             PVStructure::shared_pointer const & pv,
             const std::string& field, std::vector<uint64_t>& container) {
  
  PVField::shared_pointer v = pv->getSubField(field);
  if (v.get() == 0) {
    std::cerr << "no " << field << " field" << std::endl;
    return container;
  }

  PVScalarArray::shared_pointer a = pv->getSubField<PVScalarArray>(field);
  shared_vector<const double> cvalues;
  a->getAs(cvalues);
  container.resize(a->getLength());
 
  std::copy(cvalues.begin(), cvalues.end(), container.begin());
  return container;
}

