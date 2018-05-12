#ifndef DATASTRUCTURES_SAF_ALN_INFO_H_
#define DATASTRUCTURES_SAF_ALN_INFO_H_

#include <cstdint>
#include <vector>

#include <pbdata/Types.h>
#include <pbdata/alignment/CmpAlignment.hpp>

class AlnInfo
{
public:
    std::vector<CmpAlignment> alignments;
    UInt nAlignments;
    uint64_t lastRow;
};

#endif
