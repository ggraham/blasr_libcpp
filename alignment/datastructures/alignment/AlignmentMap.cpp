#include <alignment/datastructures/alignment/AlignmentMap.hpp>

#include <string>
#include <vector>

void CreateSequenceToAlignmentMap(const std::string& alignedSequence,
                                  std::vector<int>& baseToAlignmentMap)
{
    baseToAlignmentMap.resize(alignedSequence.size());
    size_t alignedPos, unalignedPos;
    for (alignedPos = 0, unalignedPos = 0; alignedPos < alignedSequence.size(); alignedPos++) {
        if (!(alignedSequence[alignedPos] == ' ' || alignedSequence[alignedPos] == '-')) {
            baseToAlignmentMap[unalignedPos] = alignedPos;
            unalignedPos++;
        }
    }
    baseToAlignmentMap.resize(unalignedPos);
}
