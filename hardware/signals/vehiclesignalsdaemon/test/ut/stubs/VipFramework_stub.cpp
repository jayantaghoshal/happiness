
#include "dataelements/vipcomm/VipFramework.h"

std::vector<autosar::DataElemInfo*> VipFrameworkStub_elements;

namespace vipcomm {

// get a vector that contains all available dataelements (signals)
const std::vector<autosar::DataElemInfo*>& getAllDataelements() { return VipFrameworkStub_elements; }
}
