#ifndef DATASTRUCTURES_ALIGNMENT_CMP_REF_SEQ_TABLE_H_
#define DATASTRUCTURES_ALIGNMENT_CMP_REF_SEQ_TABLE_H_

#include <string>
#include <vector>

class CmpRefSeqTable
{
public:
    void resize(int size)
    {
        refSeqNameIds.resize(size);
        refSeqNames.resize(size);
    }
    std::vector<int> refSeqNameIds;
    std::vector<std::string> refSeqNames;
    int lastRow;
};

#endif
