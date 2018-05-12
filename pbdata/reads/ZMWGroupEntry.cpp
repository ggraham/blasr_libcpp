#include <pbdata/Types.h>
#include <pbdata/reads/ZMWGroupEntry.hpp>

#include <cstdint>

ZMWGroupEntry::ZMWGroupEntry()
{
    holeNumber = x = y = 0;
    numEvents = 0;
    holeStatus = '0';
}
