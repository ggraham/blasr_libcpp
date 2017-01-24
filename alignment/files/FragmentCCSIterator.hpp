#ifndef _BLASR_FRAGMENT_CCS_ITERATOR_HPP_
#define _BLASR_FRAGMENT_CCS_ITERATOR_HPP_

#include <vector>
#include "../../pbdata/reads/RegionTable.hpp"  // pbdata
#include "../utils/RegionUtils.hpp"
#include "CCSIterator.hpp"

class FragmentCCSIterator : public CCSIterator
{
public:
    RegionTable *regionTablePtr;
    std::vector<ReadInterval> subreadIntervals;
    std::vector<int> readIntervalDirection;

    virtual void Initialize(CCSSequence *_seqPtr, RegionTable *_regionTablePtr);

    virtual int GetNext(int &direction, int &startBase, int &numBases);
};

#endif
