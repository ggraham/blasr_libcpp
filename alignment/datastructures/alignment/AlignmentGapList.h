#ifndef DATASTRUCTURES_ALIGNMENT_GAP_LIST_H_
#define DATASTRUCTURES_ALIGNMENT_GAP_LIST_H_

#include <vector>

class Gap
{
public:
    enum GapSeq
    {
        Query,
        Target
    };
    GapSeq seq;
    int length;
    Gap()
    {
        seq = Query;
        length = 0;
    }
    Gap(GapSeq seqP, int lengthP)
    {
        seq = seqP;
        length = lengthP;
    }
};

typedef std::vector<Gap> GapList;

#endif
