#ifndef DATASTRUCTURES_ALIGNMENT_CMP_READ_GROUP_TABLE_H_
#define DATASTRUCTURES_ALIGNMENT_CMP_READ_GROUP_TABLE_H_

#include <string>
#include <vector>

class CmpReadGroupTable
{
public:
    void resize(int size)
    {
        readGroupNameIds.resize(size);
        readGroupNames.resize(size);
    }
    std::vector<int> readGroupNameIds;
    std::vector<std::string> readGroupNames;
    int lastRow;
};

#endif
