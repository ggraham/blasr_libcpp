// Author: Mark Chaisson

#include <pbdata/reads/RegionAnnotation.hpp>

#include <ostream>

std::ostream& operator<<(std::ostream& os, const RegionAnnotation& ra)
{
    os << "ZMW " << ra.GetHoleNumber() << ", region type index " << ra.GetTypeIndex() << " ["
       << ra.GetStart() << ", " << ra.GetEnd() << "), " << ra.GetScore();
    return os;
}
